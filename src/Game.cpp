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

    // QObject::connect(enemy_timer, &QTimer::timeout, this, &Game::spawn_enemy);
    connect(enemy_creating_timer, &QTimer::timeout, this, &Game::spawn_enemy);

    // view->showFullScreen();
    view->show();

    display_main_menu();
}

void Game::prepare_common_dialog()
{
    if (common_dialog != nullptr)
    {
        delete common_dialog;
    }
    common_dialog = new QGraphicsRectItem();
    common_dialog->setRect(0, 0, 500, 300);
    common_dialog->setPos((width() - 500) / 2, (height() - 300) / 2);
    common_dialog->setBrush(QBrush(Qt::lightGray));
    addItem(common_dialog);
}

void Game::display_main_menu()
{
    prepare_common_dialog();
    QGraphicsTextItem *title = new QGraphicsTextItem(QString("Airplane Game"), common_dialog);
    title->setFont(QFont("sans", 18));
    title->setPos((common_dialog->rect().width() - title->boundingRect().width()) / 2, 50);

    Button *play_button = new Button("Play", common_dialog);
    play_button->setPos((common_dialog->rect().width() - play_button->boundingRect().width()) / 2, 130);
    connect(play_button, &Button::clicked, this, &Game::start_game);

    play_button->setFocus();

    Button *quit_button = new Button("Quit", common_dialog);
    quit_button->setPos((common_dialog->rect().width() - quit_button->boundingRect().width()) / 2, 190);
    connect(quit_button, &Button::clicked, view, &QGraphicsView::close);
}

void Game::display_replay_menu()
{
    prepare_common_dialog();
    QString end_text = QString("Game Over, your score: %1").arg(player->get_score());
    QGraphicsTextItem *title = new QGraphicsTextItem(end_text, common_dialog);
    title->setFont(QFont("sans", 18));
    title->setPos((common_dialog->rect().width() - title->boundingRect().width()) / 2, 50);

    Button *replay_button = new Button(QString("Re-Play"), common_dialog);
    replay_button->setPos((common_dialog->rect().width() - replay_button->boundingRect().width()) / 2, 130);
    connect(replay_button, &Button::clicked, this, &Game::start_game);

    replay_button->setFocus();

    Button *quit_button = new Button(QString("Quit"), common_dialog);
    quit_button->setPos((common_dialog->rect().width() - quit_button->boundingRect().width()) / 2, 190);
    connect(quit_button, &Button::clicked, view, &QGraphicsView::close);
}

void Game::cleanup()
{
    clear();
    /* Items inside static_items will also be cleared by `clear()`. Just clean up std::list here will be ok */
    static_items.clear();

    /*
    `Game::stop_game` will not call `clear()`, which means common_dialog should be freed by hand.
    (for now, it is done in `Game::prepare_common_dialog`)

    `Game::prepare_common_dialog` will check whether the `common_dialog` should be freed by checking
    it against with nullptr.

    So this step is really important
    */
    common_dialog = nullptr;
}

void Game::start_game()
{
    if (status == Running)
    {
        return;
    }
    cleanup();
    init_layers();

    keyboard_controller = new KeyboardController();
    addItem(keyboard_controller);
    connect(keyboard_controller, &KeyboardController::command, this, &Game::keyboard_handler);
    keyboard_controller->setFocus();

    player = new Airplane();
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
    removeItem(keyboard_controller);
    delete keyboard_controller;
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
    Enemy *enemy;
    if (random_num % 2 == 0)
    {
        enemy = new Enemy_0(layer_of_enemy);
    }
    else
    {
        enemy = new Enemy_1(layer_of_enemy);
    }
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

void Game::keyboard_handler(uint64_t command)
{
    uint64_t left_hold = command & KeyboardController::Joystick_Move_Left;
    uint64_t right_hold = command & KeyboardController::Joystick_Move_Right;
    uint64_t up_hold = command & KeyboardController::Joystick_Move_Up;
    uint64_t down_hold = command & KeyboardController::Joystick_Move_Down;
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
    if (command & KeyboardController::Joystick_Shoot)
    {
        player->shoot();
    }
    if (command & KeyboardController::Joystick_Switch_Weapon)
    {
        player->switch_weapon();
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