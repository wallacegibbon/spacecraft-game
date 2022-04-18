#ifndef __SCORE_H
#define __SCORE_H

#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include "Airplane.h"

class Score : public QGraphicsTextItem
{
public:
    Score(Airplane *_player);
    void update_score_display();

    Airplane *player;
    int score;
};

#endif