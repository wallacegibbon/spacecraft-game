#include "Game.h"
#include "Button.h"
#include "Score.h"
#include "StaticStuff.h"
#include "common.h"
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QMediaPlaylist>
#include <QRandomGenerator>
#include <QTimer>

Game::Game(int width, int height, QWidget *parent) {
  setSceneRect(0, 0, width, height);

  view_ = new QGraphicsView(this);
  // view_->setScene(this);
  view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view_->setFixedSize(width, height);

  explosion_sound_1_ = new CuteSoundPlayer("qrc:/sound/explosion_1.wav", this);
  explosion_sound_2_ = new CuteSoundPlayer("qrc:/sound/explosion_2.wav", this);

  bgm_sound_ = new CircleAudioPlayer("qrc:/sound/bgm_0.ogg", this);

  // setBackgroundBrush(QBrush(QImage(":/image/background_0.png")));
  setBackgroundBrush(QBrush(Qt::black));

  init_layers();

  // QGraphicsPixmapItem *background_img =
  //   new QGraphicsPixmapItem(QPixmap(":/image/background_0.png"));

  // add_item_to_layer(background_img, 0);

  connect(this, &Game::score_change, this, &Game::update_score);
  connect(this, &Game::enemy_destroyed, this, &Game::play_enemy_explosion);

  connect(this, &Game::start, this, &Game::start_game);
  connect(this, &Game::stop, this, &Game::stop_game);

  connect(this, &Game::refocus_keyboard, this, &Game::handle_refocus_keyboard);

  refresh_timer_ = new QTimer(this);
  enemy_creating_timer_ = new QTimer(this);

  connect(refresh_timer_, &QTimer::timeout, this, &Game::static_item_handler);

  // QObject::connect(enemy_timer, &QTimer::timeout, this, &Game::spawn_enemy);

  connect(enemy_creating_timer_, &QTimer::timeout, this, &Game::spawn_enemy);

  // view_->showFullScreen();
  view_->show();

  display_main_menu();
}

void Game::handle_refocus_keyboard() const { keyboard_controller_->setFocus(); }

void Game::prepare_common_dialog() {
  if (common_dialog_ != nullptr) { delete common_dialog_; }

  common_dialog_ = new QGraphicsRectItem();
  common_dialog_->setRect(0, 0, 500, 300);
  common_dialog_->setPos((width() - 500) / 2, (height() - 300) / 2);
  common_dialog_->setBrush(QBrush(Qt::lightGray));
  addItem(common_dialog_);
}

void Game::display_main_menu() {
  prepare_common_dialog();
  QGraphicsTextItem *title =
    new QGraphicsTextItem(QString("Spacecraft Game"), common_dialog_);

  title->setFont(QFont("sans", 18));
  title->setPos(
    (common_dialog_->rect().width() - title->boundingRect().width()) / 2, 50
  );

  Button *play_button = new Button("Play", common_dialog_);
  play_button->setPos(
    (common_dialog_->rect().width() - play_button->boundingRect().width()) / 2,
    130
  );

  connect(play_button, &Button::clicked, this, &Game::start_game);

  play_button->setFocus();

  Button *quit_button = new Button("Quit", common_dialog_);
  quit_button->setPos(
    (common_dialog_->rect().width() - quit_button->boundingRect().width()) / 2,
    190
  );

  connect(quit_button, &Button::clicked, view_, &QGraphicsView::close);
}

void Game::display_replay_menu() {
  prepare_common_dialog();
  QString end_text =
    QString("Game Over, your score: %1").arg(player_->get_score());

  QGraphicsTextItem *title = new QGraphicsTextItem(end_text, common_dialog_);

  title->setFont(QFont("sans", 18));
  title->setPos(
    (common_dialog_->rect().width() - title->boundingRect().width()) / 2, 50
  );

  Button *replay_button = new Button(QString("Re-Play"), common_dialog_);

  replay_button->setPos(
    (common_dialog_->rect().width() - replay_button->boundingRect().width()) /
      2,
    130
  );

  connect(replay_button, &Button::clicked, this, &Game::start_game);

  replay_button->setFocus();

  Button *quit_button = new Button(QString("Quit"), common_dialog_);
  quit_button->setPos(
    (common_dialog_->rect().width() - quit_button->boundingRect().width()) / 2,
    190
  );

  connect(quit_button, &Button::clicked, view_, &QGraphicsView::close);
}

void Game::cleanup() {
  clear();
  /// Items inside static_items will also be cleared by `clear()`.
  /// Just clean up std::list here will be ok
  static_items_.clear();

  /// `Game::stop_game` will not call `clear()`,
  /// which means common_dialog_ should be freed by hand.
  /// (for now, it is done in `Game::prepare_common_dialog`
  ///
  /// `Game::prepare_common_dialog` will check whether the
  /// `common_dialog_` should be freed by checking
  /// it against with nullptr
  ///
  /// So this step is really important
  common_dialog_ = nullptr;
}

void Game::start_game() {
  if (status_ == Running) { return; }

  cleanup();
  init_layers();

  keyboard_controller_ = new KeyboardController();
  addItem(keyboard_controller_);
  connect(
    keyboard_controller_,
    &KeyboardController::command,
    this,
    &Game::keyboard_handler
  );

  keyboard_controller_->setFocus();

  player_ = new Spacecraft();
  player_->setPos(
    view_->width() / 2 - player_->width() / 2,
    view_->height() - player_->height() - 10
  );

  add_item_to_layer(player_, 2);

  score_ = new Score(player_);
  add_item_to_layer(score_, NumOfLayers - 1);

  refresh_timer_->start(BASE_INTERVAL);
  enemy_creating_timer_->start(enemy_creating_interval_);

  bgm_sound_->play();
  status_ = Running;
}

void Game::stop_game() {
  if (status_ == Stopped) { return; }

  status_ = Stopped;
  bgm_sound_->stop();
  refresh_timer_->stop();
  enemy_creating_timer_->stop();
  explosion_sound_2_->play();
  removeItem(keyboard_controller_);
  delete keyboard_controller_;
  keyboard_controller_ = nullptr;
  display_replay_menu();
}

void Game::init_layers() {
  for (QGraphicsRectItem *&layer : layers_) {
    layer = new QGraphicsRectItem();
    addItem(layer);
  }
}

void Game::spawn_enemy() {
  int random_num = QRandomGenerator::global()->bounded(0, 10);
  /// the player is in layer2, enemy should be in layer 1 or 3
  int layer_of_enemy = random_num > 5 ? 1 : 3;
  Enemy *enemy;
  if (random_num % 2 == 0) {
    enemy = new Enemy_0(layer_of_enemy);
  } else {
    enemy = new Enemy_1(layer_of_enemy);
  }

  add_item_to_layer(enemy, layer_of_enemy);
  if (random_num > 4) {
    add_static_item(new RandomStaticDust(), NumOfLayers - 2);
  }
}

void Game::update_score() { score_->update_score_display(); }

void Game::play_enemy_explosion() const { explosion_sound_1_->play(); }

void Game::keyboard_handler(uint64_t command) const {
  uint64_t left_hold = command & KeyboardController::Joystick_Move_Left;
  uint64_t right_hold = command & KeyboardController::Joystick_Move_Right;
  uint64_t up_hold = command & KeyboardController::Joystick_Move_Up;
  uint64_t down_hold = command & KeyboardController::Joystick_Move_Down;
  if (left_hold && !right_hold) { player_->move_left(8); }
  if (right_hold && !left_hold) { player_->move_right(8); }
  if ((left_hold && right_hold) || (!left_hold && !right_hold)) {
    player_->backto_normal_direction();
  }
  if (up_hold && !down_hold) { player_->move_up(8); }
  if (down_hold && !up_hold) { player_->move_down(STATIC_Y_STEP - 1); }
  if ((up_hold && down_hold) || (!up_hold && !down_hold)) {
    player_->backto_normal_speed();
  }
  if (command & KeyboardController::Joystick_Shoot) { player_->shoot(); }
  if (command & KeyboardController::Joystick_Switch_Weapon) {
    player_->switch_weapon();
  }
}

bool Game::add_item_to_layer(QGraphicsItem *item, int layer_num) {
  if (layer_num >= NumOfLayers) { return false; }

  item->setParentItem(layers_[layer_num]);
  return true;
}

void Game::add_static_item(QGraphicsItem *item, int layer) {
  add_item_to_layer(item, layer);
  static_items_.push_front(item);
}

void Game::static_item_handler() {
  if (static_items_.empty()) { return; }

  std::list<QGraphicsItem *>::iterator it = static_items_.begin();
  while (it != static_items_.end()) {
    QGraphicsItem *item = *it;
    item->setPos(item->x(), item->y() + STATIC_Y_STEP);
    if (item->y() > height()) {
      it = static_items_.erase(it);
      removeItem(item);
      delete item;
    } else {
      it++;
    }
  }
}

void Game::clear_static_items() {
  for (QGraphicsItem *item : static_items_) {
    removeItem(item);
    delete item;
  }
  static_items_.clear();
}
