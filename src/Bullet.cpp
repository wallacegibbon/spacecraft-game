#include "Bullet.h"
#include "Enemy.h"
#include "Game.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTimer>

extern Game *game;

Bullet::Bullet(
  QString image,
  QString sound,
  int damage,
  int max_shoot_interval,
  Spacecraft *player
)
  : image_(image), sound_(sound), damage_(damage),
    max_shoot_interval_(max_shoot_interval), player_(player) {
  setPixmap(QPixmap(image_));
  connect(game->get_refresh_timer(), &QTimer::timeout, this, &Bullet::move);
}

void Bullet::handle_hit(Enemy *enemy) const {
  emit enemy->hit_by_bullet(player_, damage_);
  game->removeItem(const_cast<Bullet *>(this));
  delete this;
}

int Bullet::check_hit_and_handle() const {
  int collided = 0;
  QList<QGraphicsItem *> colliding_items = collidingItems();
  for (QGraphicsItem *item : colliding_items) {
    if (dynamic_cast<Enemy *>(item) != nullptr) {
      handle_hit(dynamic_cast<Enemy *>(item));
      // collided++;
      return 1;
    }
  }
  return collided;
}

bool Bullet::should_be_destroyed() const {
  return (y() + height() < 0) || (y() > game->height()) ||
         (x() + width() < 0) || (x() > game->width());
}

void Bullet::move() {
  int collided = check_hit_and_handle();
  if (collided > 0) { return; }

  setPos(x(), y() - 15);
  if (should_be_destroyed()) {
    game->removeItem(this);
    delete this;
  }
}
