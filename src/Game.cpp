#include "Game.h"
#include "CuteSoundPlayer.h"
#include "Score.h"
#include "StaticStuff.h"
#include "common.h"
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QKeyEvent>
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

    explosion_sound = new CuteSoundPlayer("qrc:/sound/explosion_0.wav");

    QMediaPlaylist *bgm_playlist = new QMediaPlaylist();
    bgm_playlist->addMedia(QUrl("qrc:/sound/bgm_0.ogg"));
    bgm_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    bgm_sound = new CuteSoundPlayer();
    bgm_sound->setPlaylist(bgm_playlist);
    bgm_sound->play();

    // setBackgroundBrush(QBrush(QImage(":/image/background_0.png")));
    setBackgroundBrush(QBrush(Qt::black));

    for (QGraphicsRectItem *&layer : layers)
    {
        layer = new QGraphicsRectItem();
        addItem(layer);
    }

    QGraphicsPixmapItem *background_img = new QGraphicsPixmapItem(QPixmap(":/image/background_0.png"));
    add_item_to_layer(background_img, 0);

    player = new Airplane();
    // player->setFlag(QGraphicsItem::ItemIsFocusable);
    // player->setFocus();
    player->setPos(view->width() / 2 - player->width() / 2, view->height() - player->height() - 10);
    add_item_to_layer(player, 2);

    score = new Score(player);
    add_item_to_layer(score, NumOfLayers - 1);

    connect(this, &Game::score_change, this, &Game::update_score);
    connect(this, &Game::enemy_destroyed, this, &Game::play_enemy_explosion);

    refresh_timer = new QTimer(this);
    refresh_timer->start(BASE_INTERVAL);

    connect(refresh_timer, &QTimer::timeout, this, &Game::static_item_handler);
    connect(refresh_timer, &QTimer::timeout, this, &Game::keyboard_handler);

    QTimer *enemy_timer = new QTimer(this);
    enemy_timer->start(1200);

    // QObject::connect(enemy_timer, &QTimer::timeout, this, &Game::spawn_enemy);
    connect(enemy_timer, &QTimer::timeout, this, &Game::spawn_enemy);
}

void Game::show()
{
    // view->showFullScreen();
    view->show();
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
    explosion_sound->play();
}

uint64_t common_key_prepare(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return Game::Joystick_Empty_Command;
    }
    switch (event->key())
    {
    case Qt::Key_Left: return Game::Joystick_Move_Left;
    case Qt::Key_Right: return Game::Joystick_Move_Right;
    case Qt::Key_Up: return Game::Joystick_Move_Up;
    case Qt::Key_Down: return Game::Joystick_Move_Down;
    case Qt::Key_Space: return Game::Joystick_Shoot_1;
    default: return Game::Joystick_Empty_Command;
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