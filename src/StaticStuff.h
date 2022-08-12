#ifndef __STATIC_STUFF_H
#define __STATIC_STUFF_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

class RandomStaticDust : public QGraphicsPixmapItem {
public:
	RandomStaticDust(QGraphicsItem *parent = nullptr);

private:
	int dust_id = 0;
	int dust_id_nums = 7;
};

#endif
