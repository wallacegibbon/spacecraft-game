#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QDebug>
#include "Bullet.h"
#include "Enemy.h"

Bullet::Bullet()
{
    setRect(0, 0, 10, 50);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Bullet::move);

    timer->start(50);
}

void Bullet::destroy_with(QGraphicsItem *item)
{
    scene()->removeItem(item);
    delete item;
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
            destroy_with(item);
            collided++;
        }
    }
    return collided;
}

void Bullet::move()
{
    int collided = check_and_destroy();
    if (collided > 0)
    {
        return;
    }
    setPos(x(), y() - 10);
    if (y() + rect().height() < 0)
    {
        scene()->removeItem(this);
        delete this;
    }
}