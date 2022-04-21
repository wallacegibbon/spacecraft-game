#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
#include <QKeyEvent>
#include "Game.h"
#include "Score.h"
#include "common.h"

Game::Game(int width, int height, QWidget *parent)
{
    setSceneRect(0, 0, width, height);

    view = new QGraphicsView(this);
    // view->setScene(this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(width, height);

    explosion_sound = new QMediaPlayer();
    explosion_sound->setMedia(QUrl("qrc:/sound/explosion_1.wav"));

    QMediaPlaylist *bgm_playlist = new QMediaPlaylist();
    bgm_playlist->addMedia(QUrl("qrc:/sound/bgm_1.ogg"));
    bgm_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    bgm_sound = new QMediaPlayer();
    bgm_sound->setPlaylist(bgm_playlist);
    bgm_sound->play();

    player = new Airplane();
    // player->setFlag(QGraphicsItem::ItemIsFocusable);
    // player->setFocus();
    player->setPos(view->width() / 2 - player->width() / 2, view->height() - player->height() - 10);
    addItem(player);

    score = new Score(player);
    addItem(score);

    QTimer *enemy_timer = new QTimer(this);
    // QObject::connect(enemy_timer, &QTimer::timeout, this, &Game::spawn_enemy);
    connect(enemy_timer, &QTimer::timeout, this, &Game::spawn_enemy);
    enemy_timer->start(1200);

    connect(this, &Game::score_change, this, &Game::update_score);
    connect(this, &Game::enemy_destroyed, this, &Game::play_enemy_explosion);

    QTimer *keyboard_timer = new QTimer(this);
    connect(keyboard_timer, &QTimer::timeout, this, &Game::keyboard_handler);
    keyboard_timer->start(50);

    QTimer *static_item_timer = new QTimer(this);
    connect(static_item_timer, &QTimer::timeout, this, &Game::static_item_handler);
    static_item_timer->start(BASE_INTERVAL);
}

void Game::show()
{
    // view->showFullScreen();
    view->show();
}

void Game::spawn_enemy()
{
    Enemy *enemy = new Enemy();
    addItem(enemy);
}

void Game::update_score()
{
    score->update_score_display();
}

void Game::play_enemy_explosion()
{
    if (explosion_sound->state() == QMediaPlayer::PlayingState)
    {
        explosion_sound->setPosition(0);
    }
    else if (explosion_sound->state() == QMediaPlayer::StoppedState)
    {
        explosion_sound->play();
    }
}

uint64_t common_key_prepare(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return Game::Joystick_Empty_Command;
    }
    switch (event->key())
    {
    case Qt::Key_Left:
        return Game::Joystick_Move_Left;
    case Qt::Key_Right:
        return Game::Joystick_Move_Right;
    case Qt::Key_Up:
        return Game::Joystick_Move_Up;
    case Qt::Key_Down:
        return Game::Joystick_Move_Down;
    case Qt::Key_Space:
        return Game::Joystick_Shoot_1;
    default:
        return Game::Joystick_Empty_Command;
    }
}

void Game::keyPressEvent(QKeyEvent *event)
{
    player_ctrl |= common_key_prepare(event);
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    int released_key = common_key_prepare(event);
    if (released_key == Joystick_Move_Left || released_key == Joystick_Move_Right)
    {
        player->backto_normal();
    }
    player_ctrl &= ~released_key;
}

void Game::keyboard_handler()
{
    uint64_t left_hold = player_ctrl & Joystick_Move_Left;
    uint64_t right_hold = player_ctrl & Joystick_Move_Right;
    if (left_hold && !right_hold)
    {
        player->move_left(10);
    }
    if (right_hold && !left_hold)
    {
        player->move_right(10);
    }
    if (left_hold && right_hold)
    {
        player->backto_normal();
    }
    if (player_ctrl & Joystick_Move_Up)
    {
        player->move_up(10);
    }
    if (player_ctrl & Joystick_Move_Down)
    {
        player->move_down(10);
    }
    if (player_ctrl & Joystick_Shoot_1)
    {
        player->shoot();
    }
}

void Game::add_static_item(QGraphicsItem *item)
{
    addItem(item);
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
        item->setPos(item->x(), item->y() + 5);
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