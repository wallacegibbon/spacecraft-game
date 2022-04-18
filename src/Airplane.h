#ifndef __MY_RECT_H
#define __MY_RECT_H

#include <QGraphicsRectItem>
#include <QMediaPlayer>
#include <QGraphicsItem>

class Airplane : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Airplane(QGraphicsItem *parent = nullptr);

    void inc_score(int num) { score += num; }
    int get_score() { return score; }

    void move_left(int distance);
    void move_right(int distance);
    void move_up(int distance);
    void move_down(int distance);
    void shoot();

private:
    void play_bullect_sound();

    int score = 0;
    QMediaPlayer *bullet_sound;
};

#endif