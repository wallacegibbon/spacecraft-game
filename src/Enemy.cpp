#include "Enemy.h"
#include "Airplane.h"
#include "Game.h"
#include "StaticExplosion.h"
#include "common.h"
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QTimer>
#include <algorithm>

extern Game *game;

Enemy::Enemy(QString _image, int _life, int _pay, int _layer)
	: image(_image), life(_life), pay(_pay), layer(_layer)
{
	connect(this, &Enemy::hit_by_bullet, this, &Enemy::handle_bullet_hit);
	y_step = QRandomGenerator::global()->bounded(1, STATIC_Y_STEP + 5);

	life_bar = new LifeBar(life, this);

	body = new QGraphicsPixmapItem(this);
	QPixmap pixmap(image);
	body->setPixmap(pixmap);

	int width = std::max<double>(body->boundingRect().width(),
		life_bar->width());

	int height = body->boundingRect().height() + life_bar->height() + 10;

	if (life_bar->width() == width) {
		body->setPos((width - body->boundingRect().width()) / 2,
			life_bar->height() + 10);
	} else {
		life_bar->setPos((width - life_bar->width()) / 2, 0);
		body->setPos(0, life_bar->height() + 10);
	}

	setRect(0, 0, width, height);
	setPen(QPen(Qt::NoPen));

	setPos(QRandomGenerator::global()->bounded(0, game->width()),
		-pixmap.height());

	setTransformOriginPoint(rect().width() / 2, rect().height() / 2);

	if (y_step > STATIC_Y_STEP)
		setRotation(180);

	connect(game->get_refresh_timer(), &QTimer::timeout,
		this, &Enemy::move);
}

void Enemy::move() {
	setPos(x(), y() + y_step);
	if (y() > game->height()) {
		game->removeItem(this);
		delete this;
	}
}

void Enemy::handle_bullet_hit(Airplane *player, int damage) {
	if (life > damage) {
		life -= damage;
		life_bar->update_value(life);
		return;
	}

	if (player != nullptr)
		player->inc_score(pay);

	emit game->score_change();
	emit game->enemy_destroyed();

	// create and attach the explosion animation
	StaticExplosion *explosion = new StaticExplosion_0();
	explosion->set_v_size(width(), height());
	explosion->setPos(x(), y());
	game->add_static_item(explosion, layer);

	game->removeItem(this);
	delete this;
}
