#include "StaticExplosion.h"
#include "Game.h"
#include <QDebug>
#include <QTimer>

extern Game *game;

StaticExplosion::StaticExplosion(
	int id, int total_frames, QGraphicsItem *parent
)
	: id_(id), total_frames_(total_frames), QObject(),
		QGraphicsPixmapItem(parent) {

	connect(
		game->get_refresh_timer(),
		&QTimer::timeout,
		this,
		&StaticExplosion::animation_update
	);
}

void StaticExplosion::animation_update() {
	if (current_frame_ >= total_frames_) return;

	set_animation_image(current_frame_++);
}

void StaticExplosion::set_v_size(int x, int y) {
	v_width_ = x;
	v_height_ = y;
}

void StaticExplosion::set_animation_image(int index) {
	QPixmap pixmap(QString(":/image/explosion_animation_%1/%2.png")
		.arg(id_)
		.arg(index, 4, 10, QLatin1Char('0')));

	int x = (v_width_ - pixmap.size().width()) / 2;
	int y = (v_height_ - pixmap.size().height()) / 2;
	setOffset(x, y);
	setPixmap(pixmap);
}
