#ifndef __GAME_H
#define __GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QObject>
#include "Airplane.h"
#include "Enemy.h"
#include "Score.h"

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);
    void show();
    void spawn_enemy();
    void update_score();

signals:
    void score_change();

private:
    QGraphicsView *view;
    Airplane *player;
    Score *score;
};

#endif