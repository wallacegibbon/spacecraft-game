#include "Spacecraft.h"
#include "Bullet.h"
#include "Game.h"
#include <QDateTime>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>

extern Game *game;

Spacecraft::Spacecraft(QGraphicsItem *parent) {
  bullet_sound_ = new CuteSoundPlayer(this);
  connect(
    game->get_refresh_timer(), &QTimer::timeout, this, &Spacecraft::check_hit
  );
  draw();
  setFlag(QGraphicsItem::ItemIsFocusable);
}

void Spacecraft::draw() {
  QPixmap body(body_image_url());
  QPixmap flame(speed_image_url());
  QPixmap full(body.width(), body.height() + flame.height());
  full.fill(Qt::transparent);
  QPainter painter(&full);
  painter.drawPixmap(
    (body.width() - flame.width()) / 2, body.height() + flame_offset_, flame
  );
  painter.drawPixmap(0, 0, body);
  setPixmap(full);
  update_flame_cnt();
}

void Spacecraft::check_hit() const {
  QList<QGraphicsItem *> colliding_items = collidingItems();
  for (QGraphicsItem *item : colliding_items) {
    if (dynamic_cast<Enemy *>(item) != nullptr) { emit game->stop(); }
  }
}

QString Spacecraft::body_image_url() const {
  switch (direction_) {
  case Left:
    return QString(":/image/spacecraft_%1/left.png").arg(Spacecraft_id_);
  case Right:
    return QString(":/image/spacecraft_%1/right.png").arg(Spacecraft_id_);
  default:
    return QString(":/image/spacecraft_%1/normal.png").arg(Spacecraft_id_);
  }
}

QString Spacecraft::speed_image_url() const {
  switch (speed_) {
  case Fast:
    return QString(":/image/rocket_flame_%1/fast_%2.png")
      .arg(flame_id_)
      .arg(flame_cnt_);
  case Slow:
    return QString(":/image/rocket_flame_%1/slow_%2.png")
      .arg(flame_id_)
      .arg(flame_cnt_);
  default:
    return QString(":/image/rocket_flame_%1/normal_%2.png")
      .arg(flame_id_)
      .arg(flame_cnt_);
  }
}

void Spacecraft::update_flame_cnt() {
  flame_cnt_++;
  if (flame_cnt_ >= flame_cnt_total_) { flame_cnt_ = 0; }
}

void Spacecraft::shoot() {
  Bullet *new_bullet = new_bullet_of_type(bullet_type_, this);
  int64_t now = QDateTime::currentMSecsSinceEpoch();
  // qDebug() << "max shoot interval:" << new_bullet->get_max_shoot_interval();
  if (now - previous_shoot_time_ > new_bullet->get_max_shoot_interval()) {
    previous_shoot_time_ = now;
  } else {
    delete new_bullet;
    return;
  }
  new_bullet->setPos(x() + width() / 2 - new_bullet->width() / 2, y());
  game->add_item_to_layer(new_bullet, 1);
  bullet_sound_->setMedia(QUrl(new_bullet->get_sound()));
  bullet_sound_->play();
}

void Spacecraft::switch_weapon() {
  int64_t now = QDateTime::currentMSecsSinceEpoch();
  if (now - previous_weapon_switch_time_ > weapon_switch_interval_) {
    previous_weapon_switch_time_ = now;
  } else {
    return;
  }

  if (bullet_type_ == BULLET_0) {
    bullet_type_ = BULLET_1;
  } else {
    bullet_type_ = BULLET_0;
  }
}

void Spacecraft::backto_normal_direction() {
  direction_ = Normal;
  draw();
}

void Spacecraft::backto_normal_speed() {
  speed_ = Default;
  draw();
}

void Spacecraft::move_left(int distance) {
  direction_ = Left;
  draw();
  if (x() > 0) { setPos(x() - distance, y()); }
}

void Spacecraft::move_right(int distance) {
  direction_ = Right;
  draw();
  if (x() + width() < game->width()) { setPos(x() + distance, y()); }
}

void Spacecraft::move_up(int distance) {
  speed_ = Fast;
  draw();
  if (y() > 10) { setPos(x(), y() - distance); }
}

void Spacecraft::move_down(int distance) {
  speed_ = Slow;
  draw();
  if (y() < game->height() - height() - 10) { setPos(x(), y() + distance); }
}

void Spacecraft::focusInEvent(QFocusEvent *event) {
  emit game->refocus_keyboard();
}
