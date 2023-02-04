#include "StaticStuff.h"
#include "Game.h"
#include "common.h"
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <algorithm>

extern Game *game;

RandomStaticDust::RandomStaticDust(QGraphicsItem *parent)
  : QGraphicsPixmapItem(parent) {
  /// rotation angle bigger than 90 degree will make the
  /// out-of-view-checking complex
  int random_angle = QRandomGenerator::global()->bounded(0, 90);
  dust_id_ = random_angle % dust_id_nums_;
  QPixmap pixmap(QString(":/image/dust_%1").arg(dust_id_));
  setPixmap(pixmap);
  int y_offset = -std::max(
    hypotenuse(boundingRect().height(), boundingRect().width()), game->height()
  );

  setPos(
    QRandomGenerator::global()->bounded(0, game->width() + 60) - 30, y_offset
  );

  setRotation(random_angle);
}
