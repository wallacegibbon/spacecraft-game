#ifndef __ENEMY_H
#define __ENEMY_H

#include <QObject>
#include <QGraphicsRectItem>

class Enemy : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Enemy();
    void move();
    int get_score() { return score; }

private:
    int size;
    int score;
};

#endif