#pragma once

#include "LifeBar.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

class Spacecraft;

class Enemy : public QObject, public QGraphicsRectItem {
	Q_OBJECT

public:
	Enemy(QString image, int life, int pay, int layer);
	void move();
	int get_layer() const { return layer_; }
	qreal height() const { return boundingRect().height(); }
	qreal width() const { return boundingRect().width(); }
	void handle_bullet_hit(Spacecraft *player, int damage);

signals:
	void hit_by_bullet(Spacecraft *player, int damage);

private:
	QGraphicsPixmapItem *body_;
	LifeBar *life_bar_;
	QString image_;
	int life_;
	int pay_;
	int y_step_;
	int layer_;
};

class Enemy_0 : public Enemy {
public:
	Enemy_0(int layer) : Enemy(":/image/enemy_0.png", 8, 1, layer) {}
};

class Enemy_1 : public Enemy {
public:
	Enemy_1(int layer) : Enemy(":/image/enemy_1.png", 12, 2, layer) {}
};
