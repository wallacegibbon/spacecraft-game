#ifndef __GAME_H
#define __GAME_H

#include "Airplane.h"
#include "CircleAudioPlayer.h"
#include "CuteSoundPlayer.h"
#include "Enemy.h"
#include "Score.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <list>

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    // clang-format off
    enum Status { Running, Stopped };
    // clang-format on

    Game(int width, int height, QWidget *parent = nullptr);
    Airplane *get_player() { return player; }
    void add_static_item(QGraphicsItem *item, int layer);
    bool add_item_to_layer(QGraphicsItem *item, int layer_num);
    QTimer *get_refresh_timer() { return refresh_timer; }
    void start_game();
    void stop_game();

signals:
    void start();
    void stop();

private:
    static constexpr uint64_t Joystick_Empty_Command = 0;
    static constexpr uint64_t Joystick_Move_Left = 1;
    static constexpr uint64_t Joystick_Move_Right = 1 << 1;
    static constexpr uint64_t Joystick_Move_Up = 1 << 2;
    static constexpr uint64_t Joystick_Move_Down = 1 << 3;
    static constexpr uint64_t Joystick_Shoot_1 = 1 << 4;
    static constexpr uint64_t Joystick_Shoot_2 = 1 << 5;

    /*
     * layer 0 for static background (far) static things;
     * layer 1 and 3 for enemies;
     * layer 2 for airplane;
     * layer 4 (or 4+) for foreground (near) static things;
     */
    static constexpr int NumOfLayers = 6;

    void init_layers();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void update_score();
    void play_enemy_explosion();
    void spawn_enemy();
    uint64_t common_key_prepare(QKeyEvent *event);
    void keyboard_handler();
    void static_item_handler();
    void clear_static_items();
    void display_main_menu();
    void display_replay_menu();
    void prepare_ui_board();

signals:
    void score_change();
    void enemy_destroyed();

private:
    QGraphicsView *view;
    Airplane *player = nullptr;
    Score *score = nullptr;
    uint64_t player_ctrl = 0;
    CircleAudioPlayer *bgm_sound;
    CuteSoundPlayer *explosion_sound_1;
    CuteSoundPlayer *explosion_sound_2;
    std::list<QGraphicsItem *> static_items;
    /* the bigger the number, the upper the layer */
    QGraphicsRectItem *layers[NumOfLayers];
    QGraphicsRectItem *ui_board = nullptr;
    QTimer *refresh_timer;
    QTimer *enemy_creating_timer;
    int enemy_creating_interval = 1200;
    Status status = Stopped;
};

#endif