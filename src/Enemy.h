#ifndef __ENEMY_H
#define __ENEMY_H

#include <QGraphicsRectItem>
#include "Airplane.h"

class Enemy : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Enemy();
    void move();
    int get_score() { return score; }
    void handle_bullet_hit(Airplane *player);
    qreal height() { return boundingRect().height(); }
    qreal width() { return boundingRect().width(); }

signals:
    void hit_by_bullet(Airplane *player);

private:
    int size;
    int score;
};

#endif