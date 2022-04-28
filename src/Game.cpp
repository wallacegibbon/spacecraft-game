#include "Game.h"
#include "Button.h"
#include "CircleAudioPlayer.h"
#include "CuteSoundPlayer.h"
#include "Score.h"
#include "StaticStuff.h"
#include "common.h"
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QImage>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QRandomGenerator>
#include <QTimer>

Game::Game(int width, int height, QWidget *parent)
{
    setSceneRect(0, 0, width, height);

    view = new QGraphicsView(this);
    // view->setScene(this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(width, height);

    explosion_sound_1 = new CuteSoundPlayer("qrc:/sound/explosion_1.wav", this);
    explosion_sound_2 = new CuteSoundPlayer("qrc:/sound/explosion_2.wav", this);

    bgm_sound = new CircleAudioPlayer("qrc:/sound/bgm_0.ogg", this);

    // setBackgroundBrush(QBrush(QImage(":/image/background_0.png")));
    setBackgroundBrush(QBrush(Qt::black));

    init_layers();

    // QGraphicsPixmapItem *background_img = new QGraphicsPixmapItem(QPixmap(":/image/background_0.png"));
    // add_item_to_layer(background_img, 0);

    connect(this, &Game::score_change, this, &Game::update_score);
    connect(this, &Game::enemy_destroyed, this, &Game::play_enemy_explosion);
    connect(this, &Game::start, this, &Game::start_game);
    connect(this, &Game::stop, this, &Game::stop_game);

    refresh_timer = new QTimer(this);
    enemy_creating_timer = new QTimer(this);

    connect(refresh_timer, &QTimer::timeout, this, &Game::static_item_handler);
    connect(refresh_timer, &QTimer::timeout, this, &Game::keyboard_handler);

    // QObject::connect(enemy_timer, &QTimer::timeout, this, &Game::spawn_enemy);
    connect(enemy_creating_timer, &QTimer::timeout, this, &Game::spawn_enemy);

    // view->showFullScreen();
    view->show();

    display_main_menu();
}

void Game::prepare_ui_board()
{
    if (ui_board != nullptr)
    {
        delete ui_board;
    }
    ui_board = new QGraphicsRectItem();
    ui_board->setRect(0, 0, 500, 300);
    ui_board->setPos((width() - 500) / 2, (height() - 300) / 2);
    ui_board->setBrush(QBrush(Qt::lightGray));
    addItem(ui_board);
}

void Game::display_main_menu()
{
    prepare_ui_board();
    QGraphicsTextItem *title = new QGraphicsTextItem(QString("Airplane Game"), ui_board);
    title->setFont(QFont("sans", 18));
    title->setPos((ui_board->rect().width() - title->boundingRect().width()) / 2, 50);

    Button *play_button = new Button("Play", ui_board);
    play_button->setPos((ui_board->rect().width() - play_button->boundingRect().width()) / 2, 130);
    connect(play_button, &Button::clicked, this, &Game::start_game);

    Button *quit_button = new Button("Quit", ui_board);
    quit_button->setPos((ui_board->rect().width() - quit_button->boundingRect().width()) / 2, 190);
    connect(quit_button, &Button::clicked, view, &QGraphicsView::close);
}

void Game::display_replay_menu()
{
    prepare_ui_board();
    QString end_text = QString("Game Over, your score: %1").arg(player->get_score());
    QGraphicsTextItem *title = new QGraphicsTextItem(end_text, ui_board);
    title->setFont(QFont("sans", 18));
    title->setPos((ui_board->rect().width() - title->boundingRect().width()) / 2, 50);

    Button *play_button = new Button(QString("Re-Play"), ui_board);
    play_button->setPos((ui_board->rect().width() - play_button->boundingRect().width()) / 2, 130);
    connect(play_button, &Button::clicked, this, &Game::start_game);

    Button *quit_button = new Button(QString("Quit"), ui_board);
    quit_button->setPos((ui_board->rect().width() - quit_button->boundingRect().width()) / 2, 190);
    connect(quit_button, &Button::clicked, view, &QGraphicsView::close);
}

void Game::start_game()
{
    if (status == Running)
    {
        return;
    }
    clear_static_items();
    clear();

    /*
    `Game::stop_game` will not call `clear()`, which means ui_board should be free by hand.
    (for now, it is done in `Game::prepare_ui_board`)

    `Game::prepare_ui_board` will check whether the `ui_board` should be freed
    by checking it against with nullptr.

    So this step is really important
    */
    ui_board = nullptr;

    init_layers();

    player = new Airplane();
    // player->setFlag(QGraphicsItem::ItemIsFocusable);
    // player->setFocus();
    player->setPos(view->width() / 2 - player->width() / 2, view->height() - player->height() - 10);
    add_item_to_layer(player, 2);

    score = new Score(player);
    add_item_to_layer(score, NumOfLayers - 1);

    refresh_timer->start(BASE_INTERVAL);
    enemy_creating_timer->start(enemy_creating_interval);

    bgm_sound->play();
    status = Running;
}

void Game::stop_game()
{
    if (status == Stopped)
    {
        return;
    }
    status = Stopped;
    bgm_sound->stop();
    refresh_timer->stop();
    enemy_creating_timer->stop();
    explosion_sound_2->play();
    display_replay_menu();
}

void Game::init_layers()
{
    for (QGraphicsRectItem *&layer : layers)
    {
        layer = new QGraphicsRectItem();
        addItem(layer);
    }
}

void Game::spawn_enemy()
{
    int random_num = QRandomGenerator::global()->bounded(0, 10);
    /* the player is in layer2, enemy should be in layer 1 or 3 */
    int layer_of_enemy = random_num > 5 ? 1 : 3;
    Enemy *enemy = new Enemy(layer_of_enemy);
    add_item_to_layer(enemy, layer_of_enemy);
    if (random_num > 4)
    {
        add_static_item(new RandomStaticDust(), NumOfLayers - 2);
    }
}

void Game::update_score()
{
    score->update_score_display();
}

void Game::play_enemy_explosion()
{
    explosion_sound_1->play();
}

uint64_t Game::common_key_prepare(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return Joystick_Empty_Command;
    }
    switch (event->key())
    {
    case Qt::Key_Left:
    case Qt::Key_A:
        return Joystick_Move_Left;
    case Qt::Key_Right:
    case Qt::Key_D:
        return Joystick_Move_Right;
    case Qt::Key_Up:
    case Qt::Key_W:
        return Joystick_Move_Up;
    case Qt::Key_Down:
    case Qt::Key_S:
        return Joystick_Move_Down;
    case Qt::Key_Space:
    case Qt::Key_F:
        return Joystick_Shoot_1;
    default:
        return Joystick_Empty_Command;
    }
}

void Game::keyPressEvent(QKeyEvent *event)
{
    player_ctrl |= common_key_prepare(event);
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    player_ctrl &= ~common_key_prepare(event);
}

void Game::keyboard_handler()
{
    uint64_t left_hold = player_ctrl & Joystick_Move_Left;
    uint64_t right_hold = player_ctrl & Joystick_Move_Right;
    uint64_t up_hold = player_ctrl & Joystick_Move_Up;
    uint64_t down_hold = player_ctrl & Joystick_Move_Down;
    if (left_hold && !right_hold)
    {
        player->move_left(8);
    }
    if (right_hold && !left_hold)
    {
        player->move_right(8);
    }
    if ((left_hold && right_hold) || (!left_hold && !right_hold))
    {
        player->backto_normal_direction();
    }
    if (up_hold && !down_hold)
    {
        player->move_up(8);
    }
    if (down_hold && !up_hold)
    {
        player->move_down(STATIC_Y_STEP - 1);
    }
    if ((up_hold && down_hold) || (!up_hold && !down_hold))
    {
        player->backto_normal_speed();
    }
    if (player_ctrl & Joystick_Shoot_1)
    {
        player->shoot();
    }
}

bool Game::add_item_to_layer(QGraphicsItem *item, int layer_num)
{
    if (layer_num >= NumOfLayers)
    {
        return false;
    }
    item->setParentItem(layers[layer_num]);
    return true;
}

void Game::add_static_item(QGraphicsItem *item, int layer)
{
    add_item_to_layer(item, layer);
    static_items.push_front(item);
}

void Game::static_item_handler()
{
    if (static_items.empty())
    {
        return;
    }
    std::list<QGraphicsItem *>::iterator it = static_items.begin();
    while (it != static_items.end())
    {
        QGraphicsItem *item = *it;
        item->setPos(item->x(), item->y() + STATIC_Y_STEP);
        if (item->y() > height())
        {
            it = static_items.erase(it);
            removeItem(item);
            delete item;
        }
        else
        {
            it++;
        }
    }
}

void Game::clear_static_items()
{
    for (QGraphicsItem *item : static_items)
    {
        removeItem(item);
        delete item;
    }
    static_items.clear();
}