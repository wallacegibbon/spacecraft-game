#ifndef __KEYBOARD_CONTROLLER_H
#define __KEYBOARD_CONTROLLER_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QTimer>

class KeyboardController : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    static constexpr uint64_t Joystick_Empty_Command = 0;
    static constexpr uint64_t Joystick_Move_Left = 1;
    static constexpr uint64_t Joystick_Move_Right = 1 << 1;
    static constexpr uint64_t Joystick_Move_Up = 1 << 2;
    static constexpr uint64_t Joystick_Move_Down = 1 << 3;
    static constexpr uint64_t Joystick_Shoot = 1 << 4;
    static constexpr uint64_t Joystick_Switch_Weapon = 1 << 5;

    KeyboardController(int _repeat_interval = 50, QGraphicsItem *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void command(uint64_t cmd);

private:
    uint64_t common_key_prepare(QKeyEvent *event) const;
    void repeat_handler();

private:
    uint64_t player_ctrl = 0;
    int repeat_interval;
    QTimer *repeat_timer;
    int press_stack = 0;
};

#endif