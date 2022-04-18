#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QDebug>
#include "Airplane.h"
#include "Bullet.h"

Airplane::Airplane(QGraphicsItem *parent)
{
    bullet_sound = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    bullet_sound->setMedia(QUrl("qrc:/sound/weapon1.wav"));
}

void Airplane::shoot()
{
    Bullet *new_bullet = new Bullet(this);
    new_bullet->setPos(x() + rect().width() / 2 - new_bullet->rect().width() / 2, y());
    scene()->addItem(new_bullet);
    play_bullect_sound();
}

void Airplane::play_bullect_sound()
{
    if (bullet_sound->state() == QMediaPlayer::PlayingState)
    {
        bullet_sound->setPosition(0);
    }
    else if (bullet_sound->state() == QMediaPlayer::StoppedState)
    {
        bullet_sound->play();
    }
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