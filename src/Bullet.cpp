#include "Bullet.h"
#include "Enemy.h"
#include "Game.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTimer>

extern Game *game;

Bullet::Bullet(Airplane *_player) : player(_player)
{
    setPixmap(QPixmap(QString::asprintf(":/image/bullet_0.png")));
    connect(game->get_refresh_timer(), &QTimer::timeout, this, &Bullet::move);
}

void Bullet::handle_hit(Enemy *enemy)
{
    emit enemy->hit_by_bullet(player);
    game->removeItem(this);
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

bool Bullet::should_be_destroyed()
{
    return (y() + height() < 0) || (y() > game->height()) || (x() + width() < 0) || (x() > game->width());
}

void Bullet::move()
{
    int collided = check_hit_and_handle();
    if (collided > 0)
    {
        return;
    }
    setPos(x(), y() - 15);
    if (should_be_destroyed())
    {
        game->removeItem(this);
        delete this;
    }
}