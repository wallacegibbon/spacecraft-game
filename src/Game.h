#ifndef __GAME_H
#define __GAME_H

#include "Airplane.h"
#include "CircleAudioPlayer.h"
#include "CuteSoundPlayer.h"
#include "Enemy.h"
#include "KeyboardController.h"
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
    /*
     * layer 0 for static background (far) static things;
     * layer 1 and 3 for enemies;
     * layer 2 for airplane;
     * layer 4 (or 4+) for foreground (near) static things;
     */
    static constexpr int NumOfLayers = 6;

    void init_layers();
    void update_score();
    void play_enemy_explosion() const;
    void spawn_enemy();
    void keyboard_handler(uint64_t command) const;
    void static_item_handler();
    void clear_static_items();
    void display_main_menu();
    void display_replay_menu();
    void prepare_common_dialog();
    void cleanup();
    void handle_refocus_keyboard() const;

signals:
    void score_change();
    void enemy_destroyed();
    void refocus_keyboard();

private:
    KeyboardController *keyboard_controller = nullptr;
    QGraphicsView *view;
    Airplane *player = nullptr;
    Score *score = nullptr;
    CircleAudioPlayer *bgm_sound;
    CuteSoundPlayer *explosion_sound_1;
    CuteSoundPlayer *explosion_sound_2;
    std::list<QGraphicsItem *> static_items;
    /* the bigger the number, the upper the layer */
    QGraphicsRectItem *layers[NumOfLayers];
    QGraphicsRectItem *common_dialog = nullptr;
    QTimer *refresh_timer;
    QTimer *enemy_creating_timer;
    int enemy_creating_interval = 1200;
    Status status = Stopped;
};

#endif