#pragma once

#include "Airplane.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class Score : public QGraphicsRectItem {
  Airplane *player_;
  QGraphicsTextItem *score_;

public:
  Score(Airplane *player);
  void update_score_display();
};
