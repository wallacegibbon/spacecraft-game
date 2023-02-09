#pragma once

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QTimer>

class KeyboardController : public QObject, public QGraphicsRectItem {
  Q_OBJECT

  uint64_t player_ctrl_ = 0;
  int repeat_interval_;
  QTimer *repeat_timer_;
  int press_stack_ = 0;

public:
  static constexpr uint64_t Joystick_Empty_Command = 0;
  static constexpr uint64_t Joystick_Move_Left = 1;
  static constexpr uint64_t Joystick_Move_Right = 1 << 1;
  static constexpr uint64_t Joystick_Move_Up = 1 << 2;
  static constexpr uint64_t Joystick_Move_Down = 1 << 3;
  static constexpr uint64_t Joystick_Shoot = 1 << 4;
  static constexpr uint64_t Joystick_Switch_Weapon = 1 << 5;

  KeyboardController(int repeat_interval = 50, QGraphicsItem *parent = nullptr);

  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

signals:
  void command(uint64_t cmd);

private:
  uint64_t common_key_prepare(QKeyEvent *event) const;
  void repeat_handler();
};
