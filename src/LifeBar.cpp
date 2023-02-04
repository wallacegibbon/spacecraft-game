#include "LifeBar.h"
#include <QBrush>
#include <QPen>

LifeBar::LifeBar(int init_value, QGraphicsItem *parent)
  : init_value_(init_value), QGraphicsRectItem(parent) {
  setRect(0, 0, bar_width_, bar_height_);
  setBrush(QBrush(Qt::darkGray));
  setPen(QPen(Qt::NoPen));
  value_bar_ = new QGraphicsRectItem(this);
  value_bar_->setBrush(QBrush(Qt::darkRed));
  value_bar_->setPen(QPen(Qt::NoPen));
  update_value(init_value_);
}

void LifeBar::update_value(int value) {
  current_value_ = value > init_value_ ? init_value_ : value;
  int current_width =
    bar_width_ * (static_cast<double>(current_value_) / init_value_);

  value_bar_->setRect(0, 0, current_width, bar_height_);
}
