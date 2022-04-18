#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include "Airplane.h"
#include "Bullet.h"

void Airplane::shoot()
{
    Bullet *new_bullet = new Bullet(this);
    new_bullet->setPos(x() + rect().width() / 2 - new_bullet->rect().width() / 2, y());
    scene()->addItem(new_bullet);
}

void Airplane::move_left(int distance)
{
    if (x() > 0)
    {
        setPos(x() - distance, y());
    }
}

void Airplane::move_right(int distance)
{
    if (x() + rect().width() < scene()->width())
    {
        setPos(x() + distance, y());
    }
}

void Airplane::move_up(int distance)
{
    if (y() > 10)
    {
        setPos(x(), y() - distance);
    }
}

void Airplane::move_down(int distance)
{
    if (y() < scene()->height() - rect().height() - 10)
    {
        setPos(x(), y() + distance);
    }
}

void Airplane::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        move_left(10);
        break;
    case Qt::Key_Right:
        move_right(10);
        break;
    case Qt::Key_Up:
        move_up(10);
        break;
    case Qt::Key_Down:
        move_down(10);
        break;
    case Qt::Key_Space:
        shoot();
        break;
    default:
        break;
    }
}