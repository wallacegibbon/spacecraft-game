#pragma once

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

class RandomStaticDust : public QGraphicsPixmapItem {
public:
	RandomStaticDust(QGraphicsItem *parent = nullptr);

private:
	int dust_id_ = 0;
	int dust_id_nums_ = 7;
};

