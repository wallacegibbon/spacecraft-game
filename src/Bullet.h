#ifndef __BULLET_H
#define __BULLET_H

#include <QGraphicsRectItem>
#include <QObject>

class Bullet : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Bullet();
    void move();

private:
    void destroy_with(QGraphicsItem *item);
    int check_and_destroy();
};

#endif