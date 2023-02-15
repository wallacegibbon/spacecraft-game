#pragma once

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

class StaticExplosion : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT

public:
	StaticExplosion(
		int id, int total_frames, QGraphicsItem *parent = nullptr
	);

	void set_v_size(int x, int y);

private:
	void animation_update();
	void set_animation_image(int index);

private:
	int id_;
	int total_frames_;
	int current_frame_ = 0;
	int v_width_ = 0;
	int v_height_ = 0;
};

class StaticExplosion_0 : public StaticExplosion {
public:
	StaticExplosion_0() : StaticExplosion(0, 32) {}
};
