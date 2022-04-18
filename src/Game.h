#ifndef __GAME_H
#define __GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QMediaPlayer>
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
    void play_enemy_explosion();

signals:
    void score_change();
    void enemy_destroyed();

private:
    QGraphicsView *view;
    Airplane *player;
    Score *score;
    QMediaPlayer *explosion_sound;
};

#endif