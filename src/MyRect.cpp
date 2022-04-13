#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include "MyRect.h"
#include "Bullet.h"
#include "Enemy.h"

void MyRect::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        if (x() > 0)
        {
            setPos(x() - 10, y());
        }
        break;
    case Qt::Key_Right:
        if (x() + rect().width() < scene()->width())
        {
            setPos(x() + 10, y());
        }
        break;
    case Qt::Key_Up:
        if (y() > 10)
        {
            setPos(x(), y() - 10);
        }
        break;
    case Qt::Key_Down:
        if (y() < scene()->height() - rect().height() - 10)
        {
            setPos(x(), y() + 10);
        }
        break;
    case Qt::Key_Space:
    {
        Bullet *new_bullet = new Bullet();
        new_bullet->setPos(x() + rect().width() / 2 - new_bullet->rect().width() / 2, y());
        scene()->addItem(new_bullet);
        break;
    }
    default:
        break;
    }
}

void MyRect::spawn()
{
    Enemy *enemy = new Enemy();
    scene()->addItem(enemy);
}