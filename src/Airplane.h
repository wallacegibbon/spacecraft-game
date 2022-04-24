#ifndef __MY_RECT_H
#define __MY_RECT_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QPainter>
#include <QTimer>

class Airplane : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    // clang-format off
    enum Direction { Normal, Left, Right };
    enum Speed { Default, Fast, Slow };
    // clang-format on

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
    void backto_normal_direction();
    void backto_normal_speed();
    void shoot();

private:
    void play_bullect_sound();
    QString body_image_url();
    QString speed_image_url();
    void draw();
    void update_flame_cnt();

private:
    int score = 0;
    int airplane_id = 0;
    int flame_id = 0;
    int flame_cnt = 0;
    int flame_cnt_total = 3;
    int flame_offset = -16;
    Direction direction = Normal;
    Speed speed = Default;
    QMediaPlayer *bullet_sound;

    /* the Game keyboard handler will trigger the refresh */
    // QTimer *refresh_timer;
};

#endif