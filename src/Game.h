#ifndef __GAME_H
#define __GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include "Airplane.h"
#include "Enemy.h"
#include "Score.h"

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(int width, int height, QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void show();
    void spawn_enemy();
    void update_score();
    void play_enemy_explosion();
    Airplane *get_player() { return player; }

signals:
    void score_change();
    void enemy_destroyed();

private:
    QGraphicsView *view;
    Airplane *player;
    Score *score;
    QMediaPlayer *bgm_sound;
    QMediaPlayer *explosion_sound;
};

#endif