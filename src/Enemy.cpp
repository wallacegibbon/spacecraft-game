#include "Enemy.h"
#include "Airplane.h"
#include "Game.h"
#include "StaticExplosion.h"
#include "common.h"
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QTimer>
#include <algorithm>

extern Game *game;

Enemy::Enemy(QString image, int life, int pay, int layer)
  : image_(image), life_(life), pay_(pay), layer_(layer) {
  connect(this, &Enemy::hit_by_bullet, this, &Enemy::handle_bullet_hit);
  y_step_ = QRandomGenerator::global()->bounded(1, STATIC_Y_STEP + 5);

  life_bar_ = new LifeBar(life_, this);

  body_ = new QGraphicsPixmapItem(this);
  QPixmap pixmap(image_);
  body_->setPixmap(pixmap);

  int width =
    std::max<double>(body_->boundingRect().width(), life_bar_->width());

  int height = body_->boundingRect().height() + life_bar_->height() + 10;

  if (life_bar_->width() == width) {
    body_->setPos(
      (width - body_->boundingRect().width()) / 2, life_bar_->height() + 10
    );
  } else {
    life_bar_->setPos((width - life_bar_->width()) / 2, 0);
    body_->setPos(0, life_bar_->height() + 10);
  }

  setRect(0, 0, width, height);
  setPen(QPen(Qt::NoPen));

  setPos(
    QRandomGenerator::global()->bounded(0, game->width()), -pixmap.height()
  );

  setTransformOriginPoint(rect().width() / 2, rect().height() / 2);

  if (y_step_ > STATIC_Y_STEP) { setRotation(180); }

  connect(game->get_refresh_timer(), &QTimer::timeout, this, &Enemy::move);
}

void Enemy::move() {
  setPos(x(), y() + y_step_);
  if (y() > game->height()) {
    game->removeItem(this);
    delete this;
  }
}

void Enemy::handle_bullet_hit(Airplane *player, int damage) {
  if (life_ > damage) {
    life_ -= damage;
    life_bar_->update_value(life_);
    return;
  }

  if (player != nullptr) { player->inc_score(pay_); }

  emit game->score_change();
  emit game->enemy_destroyed();

  // create and attach the explosion animation
  StaticExplosion *explosion = new StaticExplosion_0();
  explosion->set_v_size(width(), height());
  explosion->setPos(x(), y());
  game->add_static_item(explosion, layer_);

  game->removeItem(this);
  delete this;
}
