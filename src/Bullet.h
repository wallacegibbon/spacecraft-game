#ifndef __BULLET_H
#define __BULLET_H

#include "Enemy.h"
#include <QGraphicsPixmapItem>

// clang-format off
enum BulletType { BULLET_0, BULLET_1 };
// clang-format on

class Airplane;

class Bullet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Bullet(QString _image, QString _sound, int _max_shoot_interval = 200, Airplane *_player = nullptr);
    void move();
    Airplane *owner() { return player; }
    qreal height() { return boundingRect().height(); }
    qreal width() { return boundingRect().width(); }
    int get_max_shoot_interval() { return max_shoot_interval; }
    QString get_image() { return image; }
    QString get_sound() { return sound; }

private:
    void handle_hit(Enemy *item);
    int check_hit_and_handle();
    bool should_be_destroyed();

private:
    Airplane *player;
    int damage_value = 1;
    int type = 0;
    QString image;
    QString sound;
    int max_shoot_interval;
};

class Bullet_0 : public Bullet
{
public:
    Bullet_0(Airplane *player = nullptr) : Bullet(":/image/bullet_0.png", "qrc:/sound/weapon_0.wav", 100, player) {}
};

class Bullet_1 : public Bullet
{
public:
    Bullet_1(Airplane *player = nullptr) : Bullet(":/image/bullet_1.png", "qrc:/sound/weapon_1.wav", 300, player) {}
};

inline Bullet *new_bullet_of_type(BulletType type, Airplane *owner)
{
    switch (type)
    {
    case BULLET_0:
        return new Bullet_0(owner);
    case BULLET_1:
        return new Bullet_1(owner);
    default:
        return new Bullet_0(owner);
    }
}

#endif