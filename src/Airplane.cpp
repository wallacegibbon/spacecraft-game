#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include "Airplane.h"
#include "Bullet.h"

Airplane::Airplane(QGraphicsItem *parent)
{
    setPixmap(QPixmap(QString::asprintf(":/image/airplane_%d/normal.png", airplane_id)));
    bullet_sound = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    bullet_sound->setMedia(QUrl("qrc:/sound/weapon_1.wav"));
}

void Airplane::shoot()
{
    Bullet *new_bullet = new Bullet(this);
    new_bullet->setPos(x() + width() / 2 - new_bullet->width() / 2, y());
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

void Airplane::backto_normal()
{
    setPixmap(QPixmap(QString::asprintf(":/image/airplane_%d/normal.png", airplane_id)));
}

void Airplane::move_left(int distance)
{
    if (x() > 0)
    {
        setPixmap(QPixmap(QString::asprintf(":/image/airplane_%d/left.png", airplane_id)));
        setPos(x() - distance, y());
    }
}

void Airplane::move_right(int distance)
{
    if (x() + width() < scene()->width())
    {
        setPixmap(QPixmap(QString::asprintf(":/image/airplane_%d/right.png", airplane_id)));
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
    if (y() < scene()->height() - height() - 10)
    {
        setPos(x(), y() + distance);
    }
}