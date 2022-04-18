#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
#include <QKeyEvent>
#include "Game.h"
#include "Score.h"

Game::Game(int width, int height, QWidget *parent)
{
    setSceneRect(0, 0, width, height);

    view = new QGraphicsView(this);
    // view->setScene(this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(width, height);

    explosion_sound = new QMediaPlayer();
    explosion_sound->setMedia(QUrl("qrc:/sound/explosion1.wav"));

    QMediaPlaylist *bgm_playlist = new QMediaPlaylist();
    bgm_playlist->addMedia(QUrl("qrc:/sound/bgm1.ogg"));
    bgm_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    bgm_sound = new QMediaPlayer();
    bgm_sound->setPlaylist(bgm_playlist);
    bgm_sound->play();

    player = new Airplane();
    player->setRect(0, 0, 40, 60);
    // player->setFlag(QGraphicsItem::ItemIsFocusable);
    // player->setFocus();
    player->setPos(view->width() / 2 - player->rect().width() / 2, view->height() - player->rect().height() - 10);
    addItem(player);

    score = new Score(player);
    addItem(score);

    QTimer *enemy_timer = new QTimer();
    QObject::connect(enemy_timer, &QTimer::timeout, this, &Game::spawn_enemy);
    enemy_timer->start(2000);

    QObject::connect(this, &Game::score_change, this, &Game::update_score);
    QObject::connect(this, &Game::enemy_destroyed, this, &Game::play_enemy_explosion);

    QTimer *keyboard_timer = new QTimer();
    QObject::connect(keyboard_timer, &QTimer::timeout, this, &Game::keyboard_handler);
    keyboard_timer->start(50);
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

JoyStick common_key_prepare(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return UnknownJoyStickCommand;
    }
    switch (event->key())
    {
    case Qt::Key_Left:
        return Left;
    case Qt::Key_Right:
        return Right;
    case Qt::Key_Up:
        return Up;
    case Qt::Key_Down:
        return Down;
    case Qt::Key_Space:
        return Shoot;
    default:
        return UnknownJoyStickCommand;
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
    if (player_ctrl & Left)
    {
        player->move_left(10);
    }
    if (player_ctrl & Right)
    {
        player->move_right(10);
    }
    if (player_ctrl & Up)
    {
        player->move_up(10);
    }
    if (player_ctrl & Down)
    {
        player->move_down(10);
    }
    if (player_ctrl & Shoot)
    {
        player->shoot();
    }
}