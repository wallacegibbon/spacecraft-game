#ifndef _BUTTON_H
#define _BUTTON_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class Button : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Button(QString _text, QGraphicsItem *parent = nullptr);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    /* this one doesn't work */
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void clicked();

private:
    QGraphicsTextItem *text;
};

#endif