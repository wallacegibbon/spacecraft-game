#include <QGraphicsScene>
#include <QTimer>
#include <QRandomGenerator>
#include "Game.h"
#include "Enemy.h"
#include "Airplane.h"

extern Game *game;

Enemy::Enemy()
{
    connect(this, &Enemy::hit_by_bullet, this, &Enemy::handle_bullet_hit);

    setPos(QRandomGenerator::global()->bounded(10, 790), 0);

    size = QRandomGenerator::global()->bounded(10, 70);
    setRect(0, 0, size, size);

    score = size > 40 ? 2 : 1;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Enemy::move);

    timer->start(QRandomGenerator::global()->bounded(20, 200));
}

void Enemy::move()
{
    setPos(x(), y() + 5);
    if (y() > scene()->height())
    {
        scene()->removeItem(this);
        delete this;
    }
}

void Enemy::handle_bullet_hit(Airplane *player)
{
    player->inc_score(get_score());
    emit game->score_change();
    emit game->enemy_destroyed();
    scene()->removeItem(this);
    delete this;
}