#ifndef __SCORE_H
#define __SCORE_H

#include "Airplane.h"
#include <QGraphicsItem>
#include <QGraphicsTextItem>

class Score : public QGraphicsTextItem
{
public:
    Score(Airplane *_player);
    void update_score_display();

    Airplane *player;
    int score;
};

#endif