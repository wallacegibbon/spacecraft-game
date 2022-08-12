#ifndef __SCORE_H
#define __SCORE_H

#include "Airplane.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class Score : public QGraphicsRectItem {
public:
	Score(Airplane *_player);
	void update_score_display();

private:
	Airplane *player;
	QGraphicsTextItem *score;
};

#endif
