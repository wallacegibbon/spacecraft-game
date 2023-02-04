#pragma once

#include "Enemy.h"
#include <QGraphicsPixmapItem>

enum BulletType { BULLET_0, BULLET_1 };

class Spacecraft;

class Bullet : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT

  Spacecraft *player_;
  int damage_;
  QString image_;
  QString sound_;
  int max_shoot_interval_;

public:
  Bullet(
    QString image,
    QString sound,
    int damage = 1,
    int max_shoot_interval = 200,
    Spacecraft *player = nullptr
  );

  void move();
  Spacecraft *owner() { return player_; }
  qreal height() const { return boundingRect().height(); }
  qreal width() const { return boundingRect().width(); }

  int get_max_shoot_interval() const { return max_shoot_interval_; }
  QString get_image() const { return image_; }
  QString get_sound() const { return sound_; }

private:
  void handle_hit(Enemy *item) const;
  int check_hit_and_handle() const;
  bool should_be_destroyed() const;
};

class Bullet_0 : public Bullet {
public:
  Bullet_0(Spacecraft *player = nullptr)
    : Bullet(
        ":/image/bullet_0.png", "qrc:/sound/weapon_0.wav", 2, 100, player
      ) {}
};

class Bullet_1 : public Bullet {
public:
  Bullet_1(Spacecraft *player = nullptr)
    : Bullet(
        ":/image/bullet_1.png", "qrc:/sound/weapon_1.wav", 5, 300, player
      ) {}
};

inline Bullet *new_bullet_of_type(BulletType type, Spacecraft *owner) {
  switch (type) {
  case BULLET_0:
    return new Bullet_0(owner);
  case BULLET_1:
    return new Bullet_1(owner);
  default:
    return new Bullet_0(owner);
  }
}
