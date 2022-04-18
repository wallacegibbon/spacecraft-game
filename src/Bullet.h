#ifndef __BULLET_H
#define __BULLET_H

#include <QGraphicsRectItem>
#include "Airplane.h"
#include "Enemy.h"

class Bullet : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Bullet(Airplane *_player);
    void move();
    Airplane *owner() { return player; }

private:
    void handle_hit(Enemy *item);
    int check_hit_and_handle();
    bool out_of_scene();

    Airplane *player;
};

#endif