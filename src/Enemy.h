#ifndef __ENEMY_H
#define __ENEMY_H

#include "LifeBar.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

class Airplane;

class Enemy : public QObject, public QGraphicsRectItem {
	Q_OBJECT

public:
	Enemy(QString _image, int _life, int _pay, int _layer);
	void move();
	int get_layer() const { return layer; }
	qreal height() const { return boundingRect().height(); }
	qreal width() const { return boundingRect().width(); }
	void handle_bullet_hit(Airplane *player, int damage);

signals:
	void hit_by_bullet(Airplane *player, int damage);

private:
	QGraphicsPixmapItem *body;
	LifeBar *life_bar;
	QString image;
	int life;
	int pay;
	int y_step;
	int layer;
};

class Enemy_0 : public Enemy {
public:
	Enemy_0(int layer) : Enemy(":/image/enemy_0.png", 8, 1, layer) {}
};

class Enemy_1 : public Enemy {
public:
	Enemy_1(int layer) : Enemy(":/image/enemy_1.png", 12, 2, layer) {}
};

#endif
