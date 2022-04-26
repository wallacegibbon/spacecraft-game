#include "StaticExplosion.h"
#include "Game.h"
#include <QDebug>
#include <QTimer>

extern Game *game;

StaticExplosion::StaticExplosion(int _id, int _total_frames, QGraphicsItem *parent)
    : id(_id), total_frames(_total_frames), QObject(), QGraphicsPixmapItem(parent)
{
    connect(game->get_refresh_timer(), &QTimer::timeout, this, &StaticExplosion::animation_update);
}

void StaticExplosion::animation_update()
{
    if (current_frame >= total_frames)
    {
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
    QPixmap pixmap(QString::asprintf(":/image/explosion_animation_%d/%04d.png", id, index));
    int x = (v_width - pixmap.size().width()) / 2;
    int y = (v_height - pixmap.size().height()) / 2;
    setOffset(x, y);
    setPixmap(pixmap);
}