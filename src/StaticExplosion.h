#ifndef __STATIC_EXPLOSION_H
#define __STATIC_EXPLOSION_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QTimer>

class StaticExplosion : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    StaticExplosion(int _total_frames, const char *_animation_prefix, QGraphicsItem *parent = nullptr);
    void set_v_size(int x, int y);

private:
    void animation_update();
    void set_animation_image(int index);

    const char *animation_prefix;
    int total_frames;
    int current_frame = 0;
    int v_width = 0;
    int v_height = 0;
    QTimer *animation_timer;
};

#endif