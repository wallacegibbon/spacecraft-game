#ifndef __BULLET_H
#define __BULLET_H

#include <QGraphicsRectItem>
#include <QObject>
#include "Airplane.h"
#include "Enemy.h"

class Bullet : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Bullet(Airplane *_player);
    void move();

private:
    void destroy_with(Enemy *item);
    int check_and_destroy();
    bool out_of_scene();

    Airplane *player;
};

#endif