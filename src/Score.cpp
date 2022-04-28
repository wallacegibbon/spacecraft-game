#include "Score.h"
#include <QFont>

Score::Score(Airplane *_player) : player(_player), QGraphicsTextItem(_player)
{
    setDefaultTextColor(Qt::lightGray);
    setFont(QFont("times", 12));
    update_score_display();
}

void Score::update_score_display()
{
    setPlainText(QString("Score:\t") + QString::number(player->get_score()));
}
