#include "Score.h"
#include <QFont>

Score::Score(Airplane *_player) : player(_player), QGraphicsRectItem(_player) {
	setRect(0, 0, 200, 30);
	setBrush(QBrush(Qt::darkGray));
	score = new QGraphicsTextItem(this);
	score->setDefaultTextColor(Qt::white);
	score->setFont(QFont("times", 12));
	update_score_display();
}

void Score::update_score_display() {
	score->setPlainText(QString("Score:\t%1").arg(player->get_score()));
}
