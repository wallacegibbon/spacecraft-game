#include "Button.h"
#include <QBrush>
#include <QCursor>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QKeyEvent>

Button::Button(QString text, QGraphicsItem *parent)
  : QGraphicsRectItem(parent) {
  setRect(0, 0, 200, 50);
  text_ = new QGraphicsTextItem(text, this);
  int x = (rect().width() - text_->boundingRect().width()) / 2;
  int y = (rect().height() - text_->boundingRect().height()) / 2;
  text_->setPos(x, y);

  setAcceptHoverEvents(true);
  setBrush(QBrush(Qt::darkCyan));

  setFlag(QGraphicsItem::ItemIsFocusable);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  emit clicked();
}

void Button::keyPressEvent(QKeyEvent *event) {
  int received_key = event->key();
  /// Key_Return is the main Enter key,
  /// Key_Enter is the Enter key in number pad
  if (received_key == Qt::Key_Return || received_key == Qt::Key_Enter) {
    emit clicked();
  }
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  setBrush(QBrush(Qt::cyan));
  setCursor(QCursor(Qt::PointingHandCursor));
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
  setBrush(QBrush(Qt::darkCyan));
}
