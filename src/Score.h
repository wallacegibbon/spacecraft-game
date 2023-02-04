#pragma once

#include "Spacecraft.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class Score : public QGraphicsRectItem {
  Spacecraft *player_;
  QGraphicsTextItem *score_;

public:
  Score(Spacecraft *player);
  void update_score_display();
};
