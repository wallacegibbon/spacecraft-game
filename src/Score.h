#pragma once

#include "Spacecraft.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class Score : public QGraphicsRectItem {
public:
	Score(Spacecraft *player);
	void update_score_display();

private:
	Spacecraft *player_;
	QGraphicsTextItem *score_;
};
