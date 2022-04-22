#include <QDebug>
#include "StaticExplosion.h"

StaticExplosion::StaticExplosion(int _id, int _total_frames, QGraphicsItem *parent)
    : id(_id), total_frames(_total_frames), QObject(), QGraphicsPixmapItem(parent)
{
    animation_timer = new QTimer(this);
    connect(animation_timer, &QTimer::timeout, this, &StaticExplosion::animation_update);
    animation_timer->start(50);
}

void StaticExplosion::animation_update()
{
    if (current_frame >= total_frames)
    {
        animation_timer->stop();
        return;
    }
    set_animation_image(current_frame++);
}

void StaticExplosion::set_v_size(int x, int y)
{
    v_width = x;
    v_height = y;
}

void StaticExplosion::set_animation_image(int index)
{
    QPixmap pixmap(QString::asprintf(":/image/explosion_animation_1/%04d.png", index));
    int x = (v_width - pixmap.size().width()) / 2;
    int y = (v_height - pixmap.size().height()) / 2;
    setOffset(x, y);
    setPixmap(pixmap);
}