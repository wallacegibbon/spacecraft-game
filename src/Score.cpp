#include "Score.h"
#include <QFont>

Score::Score(Airplane *player) : player_(player), QGraphicsRectItem(player) {
  setRect(0, 0, 200, 30);
  setBrush(QBrush(Qt::darkGray));
  score_ = new QGraphicsTextItem(this);
  score_->setDefaultTextColor(Qt::white);
  score_->setFont(QFont("times", 12));
  update_score_display();
}

void Score::update_score_display() {
  score_->setPlainText(QString("Score:\t%1").arg(player_->get_score()));
}
