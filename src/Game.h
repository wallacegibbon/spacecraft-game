#pragma once

#include "Spacecraft.h"
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

class Game : public QGraphicsScene {
  Q_OBJECT

public:
  enum Status { Running, Stopped };

private:
  /// layer 0 for static background (far) static things;
  /// layer 1 and 3 for enemies;
  /// layer 2 for Spacecraft;
  /// layer 4 (or 4+) for foreground (near) static things;
  static constexpr int NumOfLayers = 6;

  KeyboardController *keyboard_controller_ = nullptr;
  QGraphicsView *view_;
  Spacecraft *player_ = nullptr;
  Score *score_ = nullptr;

  CircleAudioPlayer *bgm_sound_;
  CuteSoundPlayer *explosion_sound_1_;
  CuteSoundPlayer *explosion_sound_2_;

  std::list<QGraphicsItem *> static_items_;

  /// the bigger the number, the upper the layer
  QGraphicsRectItem *layers_[NumOfLayers];
  QGraphicsRectItem *common_dialog_ = nullptr;
  QTimer *refresh_timer_;
  QTimer *enemy_creating_timer_;

  int enemy_creating_interval_ = 1200;
  Status status_ = Stopped;

public:
  Game(int width, int height, QWidget *parent = nullptr);
  Spacecraft *get_player() { return player_; }
  void add_static_item(QGraphicsItem *item, int layer);
  bool add_item_to_layer(QGraphicsItem *item, int layer_num);
  QTimer *get_refresh_timer() { return refresh_timer_; }
  void start_game();
  void stop_game();

signals:
  void start();
  void stop();

private:
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
};
