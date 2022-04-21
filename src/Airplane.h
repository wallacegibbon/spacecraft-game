#ifndef __MY_RECT_H
#define __MY_RECT_H

#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QGraphicsItem>

class Airplane : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Airplane(QGraphicsItem *parent = nullptr);

    qreal height() { return boundingRect().height(); }
    qreal width() { return boundingRect().width(); }

    void inc_score(int num) { score += num; }
    int get_score() { return score; }

    void move_left(int distance);
    void move_right(int distance);
    void move_up(int distance);
    void move_down(int distance);
    void backto_normal();
    void shoot();

private:
    void play_bullect_sound();

    int score = 0;
    QMediaPlayer *bullet_sound;

    int airplane_id = 1;
};

#endif