#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
#include "Game.h"
#include "Score.h"

Game::Game(QWidget *parent)
{
    setSceneRect(0, 0, 800, 600);

    view = new QGraphicsView(this);
    view->setScene(this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(800, 600);

    explosion_sound = new QMediaPlayer();
    explosion_sound->setMedia(QUrl("qrc:/sound/explosion1.wav"));

    QMediaPlaylist *bgm_playlist = new QMediaPlaylist();
    bgm_playlist->addMedia(QUrl("qrc:/sound/bgm1.ogg"));
    bgm_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    bgm_sound = new QMediaPlayer();
    bgm_sound->setPlaylist(bgm_playlist);
    bgm_sound->play();

    Airplane *player = new Airplane();
    player->setRect(0, 0, 40, 60);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    addItem(player);

    score = new Score(player);
    addItem(score);

    player->setPos(view->width() / 2 - player->rect().width() / 2, view->height() - player->rect().height() - 10);

    QTimer *timer = new QTimer();

    QObject::connect(timer, &QTimer::timeout, this, &Game::spawn_enemy);
    QObject::connect(this, &Game::score_change, this, &Game::update_score);
    QObject::connect(this, &Game::enemy_destroyed, this, &Game::play_enemy_explosion);

    timer->start(2000);
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