#ifndef __STATIC_STUFF_H
#define __STATIC_STUFF_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

class RandomStaticSmoke : public QGraphicsPixmapItem
{
public:
    RandomStaticSmoke(QGraphicsItem *parent = nullptr);

private:
    int smoke_id = 0;
    int smoke_id_nums = 6;
};

#endif