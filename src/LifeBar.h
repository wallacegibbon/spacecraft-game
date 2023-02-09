#pragma once

#include <QGraphicsRectItem>

class LifeBar : public QGraphicsRectItem {
  QGraphicsRectItem *value_bar_;
  int init_value_;
  int current_value_;
  int bar_width_ = 30;
  int bar_height_ = 5;

public:
  LifeBar(int init_value, QGraphicsItem *parent = nullptr);

  void update_value(int value);
  int height() const { return rect().height(); }
  int width() const { return rect().width(); }
};
