#include "KeyboardController.h"
#include <QDebug>

Qt::Key command_keys[] = {Qt::Key_Left, Qt::Key_Right, Qt::Key_Up, Qt::Key_Down, Qt::Key_A,
                          Qt::Key_W,    Qt::Key_D,     Qt::Key_S,  Qt::Key_R,    Qt::Key_F};

KeyboardController::KeyboardController(int _repeat_interval, QGraphicsItem *parent)
    : repeat_interval(_repeat_interval), QGraphicsRectItem(parent)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    repeat_timer = new QTimer(this);
    connect(repeat_timer, &QTimer::timeout, this, &KeyboardController::repeat_handler);

    repeat_timer->start(repeat_interval);
}

void KeyboardController::repeat_handler()
{
    // qDebug() << "repeat handler is triggered, press_stack: " << press_stack;
    emit command(player_ctrl);
}

bool is_command_key(int key)
{
    bool result = false;
    for (Qt::Key k : command_keys)
    {
        if (key == k)
        {
            result = true;
        }
    }
    return result;
}

void KeyboardController::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return;
    }
    if (is_command_key(event->key()))
    {
        press_stack++;
        repeat_timer->start(repeat_interval);
    }
    // qDebug() << "key press: " << event->key() << ", press_stack: " << press_stack;
    player_ctrl |= common_key_prepare(event);
    emit command(player_ctrl);
}

void KeyboardController::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return;
    }
    if (press_stack > 0 && is_command_key(event->key()))
    {
        press_stack--;
    }
    if (press_stack == 0)
    {
        repeat_timer->stop();
    }
    // qDebug() << "key release: " << event->key() << ", press_stack: " << press_stack;
    player_ctrl &= ~common_key_prepare(event);
    emit command(player_ctrl);
}

uint64_t KeyboardController::common_key_prepare(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
    case Qt::Key_A:
        return Joystick_Move_Left;
    case Qt::Key_Right:
    case Qt::Key_D:
        return Joystick_Move_Right;
    case Qt::Key_Up:
    case Qt::Key_W:
        return Joystick_Move_Up;
    case Qt::Key_Down:
    case Qt::Key_S:
        return Joystick_Move_Down;
    case Qt::Key_Space:
    case Qt::Key_F:
        return Joystick_Shoot;
    case Qt::Key_R:
        return Joystick_Switch_Weapon;
    default:
        return Joystick_Empty_Command;
    }
}