#include "Bullet.h"
#include "Enemy.h"
#include "Game.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTimer>

Bullet::Bullet(Airplane *_player) : player(_player)
{
    setPixmap(QPixmap(QString::asprintf(":/image/bullet_0.png")));

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Bullet::move);

    timer->start(50);
}

void Bullet::handle_hit(Enemy *enemy)
{
    emit enemy->hit_by_bullet(player);
    scene()->removeItem(this);
    delete this;
}

int Bullet::check_hit_and_handle()
{
    int collided = 0;
    QList<QGraphicsItem *> colliding_items = collidingItems();
    for (QGraphicsItem *item : colliding_items)
    {
        if (typeid(*item) == typeid(Enemy))
        {
            handle_hit(dynamic_cast<Enemy *>(item));
            // collided++;
            return 1;
        }
    }
    return collided;
}

bool Bullet::out_of_scene()
{
    return (y() + height() < 0) || (y() > scene()->height()) || (x() + width() < 0) || (x() > scene()->width());
}

void Bullet::move()
{
    int collided = check_hit_and_handle();
    if (collided > 0)
    {
        return;
    }
    setPos(x(), y() - 15);
    if (out_of_scene())
    {
        scene()->removeItem(this);
        delete this;
    }
}