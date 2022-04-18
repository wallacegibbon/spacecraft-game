#ifndef __GAME_H
#define __GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include "Airplane.h"
#include "Enemy.h"
#include "Score.h"

enum JoyStick : int
{
    UnknownJoyStickCommand = 0,
    Left = 1,
    Right = 1 << 1,
    Up = 1 << 2,
    Down = 1 << 3,
    Shoot = 1 << 4,
};

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(int width, int height, QWidget *parent = nullptr);
    void show();
    Airplane *get_player() { return player; }

signals:
    void score_change();
    void enemy_destroyed();

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void update_score();
    void play_enemy_explosion();
    void spawn_enemy();
    void keyboard_handler();

    QGraphicsView *view;
    Airplane *player;
    int player_ctrl;
    Score *score;
    QMediaPlayer *bgm_sound;
    QMediaPlayer *explosion_sound;
};

#endif