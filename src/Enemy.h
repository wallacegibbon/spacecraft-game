#ifndef __ENEMY_H
#define __ENEMY_H

#include "Airplane.h"
#include <QGraphicsPixmapItem>

class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Enemy(int _layer);
    void move();
    int get_score() { return score; }
    int get_layer() { return layer; }
    void handle_bullet_hit(Airplane *player);
    qreal height() { return boundingRect().height(); }
    qreal width() { return boundingRect().width(); }

signals:
    void hit_by_bullet(Airplane *player);

private:
    int y_step;
    int score;
    int layer;
};

#endif