#ifndef __MY_RECT_H
#define __MY_RECT_H

#include <QGraphicsRectItem>
#include <QObject>

class Airplane : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    void keyPressEvent(QKeyEvent *event) override;
    void spawn();

private:
    void move_left(int distance);
    void move_right(int distance);
    void move_up(int distance);
    void move_down(int distance);
    void shoot();
};

#endif