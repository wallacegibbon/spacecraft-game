#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class Button : public QObject, public QGraphicsRectItem {
  Q_OBJECT

  QGraphicsTextItem *text_;

public:
  Button(QString text, QGraphicsItem *parent = nullptr);
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

signals:
  void clicked();
};
