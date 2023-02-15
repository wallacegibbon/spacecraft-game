#pragma once

#include "Bullet.h"
#include "CuteSoundPlayer.h"
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>

class Spacecraft : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT

public:
	Spacecraft(QGraphicsItem *parent = nullptr);

	qreal height() const { return boundingRect().height(); }
	qreal width() const { return boundingRect().width(); }

	void inc_score(int num) { score_ += num; }
	int get_score() const { return score_; }

	void move_left(int distance);
	void move_right(int distance);
	void move_up(int distance);
	void move_down(int distance);
	void backto_normal_direction();
	void backto_normal_speed();
	void shoot();
	void switch_weapon();

private:
	QString body_image_url() const;
	QString speed_image_url() const;
	void draw();
	void update_flame_cnt();
	void check_hit() const;

	void focusInEvent(QFocusEvent *event) override;

private:
	enum Direction { Normal, Left, Right };
	enum Speed { Default, Fast, Slow };

	int score_ = 0;
	int life_ = 10;
	int spacecraft_id_ = 0;
	int flame_id_ = 0;
	int flame_cnt_ = 0;
	int flame_cnt_total_ = 3;
	int flame_offset_ = -16;

	Direction direction_ = Normal;
	Speed speed_ = Default;
	CuteSoundPlayer *bullet_sound_ = nullptr;

	BulletType bullet_type_ = BULLET_0;

	/// command from KeyboardController can be too frequently
	int weapon_switch_interval_ = 500;
	int64_t previous_weapon_switch_time_ = 0;
	int64_t previous_shoot_time_ = 0;
};

