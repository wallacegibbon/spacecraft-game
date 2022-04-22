#ifndef __BULLET_H
#define __BULLET_H

#include <QGraphicsPixmapItem>
#include "Airplane.h"
#include "Enemy.h"

class Bullet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Bullet(Airplane *_player);
    void move();
    Airplane *owner() { return player; }
    qreal height() { return boundingRect().height(); }
    qreal width() { return boundingRect().width(); }

private:
    void handle_hit(Enemy *item);
    int check_hit_and_handle();
    bool out_of_scene();
    Airplane *player;
};

#endif