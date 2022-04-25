#include "StaticStuff.h"
#include "Game.h"
#include "common.h"
#include <QGraphicsScene>
#include <QRandomGenerator>

extern Game *game;

RandomStaticSmoke::RandomStaticSmoke(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    /* rotation angle bigger than 90 degree will make the out-of-view-checking complex */
    int random_angle = QRandomGenerator::global()->bounded(0, 90);
    smoke_id = random_angle % smoke_id_nums;
    QPixmap pixmap(QString::asprintf(":/image/smoke_%d", smoke_id));
    setPixmap(pixmap);
    int y_offset = -max(max(boundingRect().height(), boundingRect().width()), game->height());
    setPos(QRandomGenerator::global()->bounded(0, game->width() + 60) - 30, y_offset);
    setRotation(random_angle);
}
