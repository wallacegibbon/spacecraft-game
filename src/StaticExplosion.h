#ifndef __STATIC_EXPLOSION_H
#define __STATIC_EXPLOSION_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QTimer>

class StaticExplosion : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    StaticExplosion(int _id, int _total_frames, QGraphicsItem *parent = nullptr);
    void set_v_size(int x, int y);

private:
    void animation_update();
    void set_animation_image(int index);

    int id;
    int total_frames;
    int current_frame = 0;
    int v_width = 0;
    int v_height = 0;
    QTimer *animation_timer;
};

class StaticExplosion_0 : public StaticExplosion
{
public:
    StaticExplosion_0() : StaticExplosion(0, 32) {}
};

#endif