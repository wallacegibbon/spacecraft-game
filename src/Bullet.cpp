#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QDebug>
#include "Bullet.h"
#include "Game.h"

extern Game *game;

Bullet::Bullet(Airplane *_player) : player(_player)
{
    setRect(0, 0, 10, 50);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Bullet::move);

    timer->start(50);
}

void Bullet::destroy_with(Enemy *enemy)
{
    player->inc_score(enemy->get_score());
    emit game->score_change();

    scene()->removeItem(enemy);
    delete enemy;
    scene()->removeItem(this);
    delete this;
}

int Bullet::check_and_destroy()
{
    int collided = 0;
    QList<QGraphicsItem *> colliding_items = collidingItems();
    for (QGraphicsItem *item : colliding_items)
    {
        if (typeid(*item) == typeid(Enemy))
        {
            destroy_with(dynamic_cast<Enemy *>(item));
            // collided++;
            return 1;
        }
    }
    return collided;
}

bool Bullet::out_of_scene()
{
    return (y() + rect().height() < 0) || (y() > scene()->height()) || (x() + rect().width() < 0) || (x() > scene()->width());
}

void Bullet::move()
{
    int collided = check_and_destroy();
    if (collided > 0)
    {
        return;
    }
    setPos(x(), y() - 10);
    if (out_of_scene())
    {
        scene()->removeItem(this);
        delete this;
    }
}