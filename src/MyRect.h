#ifndef __MY_RECT_H
#define __MY_RECT_H

#include <QGraphicsRectItem>
#include <QObject>

class MyRect : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent *event) override;
    void spawn();
};

#endif