#include "StaticStuff.h"
#include "Game.h"
#include <QGraphicsScene>
#include <QRandomGenerator>

extern Game *game;

RandomStaticSmoke::RandomStaticSmoke(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    smoke_id = QRandomGenerator::global()->bounded(0, 256) % smoke_id_nums;
    setPixmap(QPixmap(QString::asprintf(":/image/smoke_%d", smoke_id)));
    setPos(QRandomGenerator::global()->bounded(0, game->width() + 60) - 30, -game->height());
}
