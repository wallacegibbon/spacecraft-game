#pragma once

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

class RandomStaticDust : public QGraphicsPixmapItem {
  int dust_id_ = 0;
  int dust_id_nums_ = 7;

public:
  RandomStaticDust(QGraphicsItem *parent = nullptr);
};
