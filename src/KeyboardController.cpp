#include "KeyboardController.h"
#include <QDebug>

Qt::Key command_keys[] = {
	Qt::Key_Left,
	Qt::Key_Right,
	Qt::Key_Up,
	Qt::Key_Down,
	Qt::Key_A,
	Qt::Key_W,
	Qt::Key_D,
	Qt::Key_S,
	Qt::Key_R,
	Qt::Key_F
};

KeyboardController::KeyboardController(
	int repeat_interval, QGraphicsItem *parent
)
	: repeat_interval_(repeat_interval), QGraphicsRectItem(parent) {

	setFlag(QGraphicsItem::ItemIsFocusable);
	repeat_timer_ = new QTimer(this);
	connect(
		repeat_timer_,
		&QTimer::timeout,
		this,
		&KeyboardController::repeat_handler
	);

	repeat_timer_->start(repeat_interval_);
}

void KeyboardController::repeat_handler() {
	emit command(player_ctrl_);
}

bool is_command_key(int key) {
	bool result = false;
	for (Qt::Key k : command_keys) {
		if (key == k) result = true;
	}
	return result;
}

void KeyboardController::keyPressEvent(QKeyEvent *event) {
	if (event->isAutoRepeat()) return;

	if (is_command_key(event->key())) {
		press_stack_++;
		repeat_timer_->start(repeat_interval_);
	}
	/*
	qDebug() << "pressed: " << event->key()
		<< ", press_stack_: " << press_stack_;
	*/
	player_ctrl_ |= common_key_prepare(event);
	emit command(player_ctrl_);
}

void KeyboardController::keyReleaseEvent(QKeyEvent *event) {
	if (event->isAutoRepeat()) return;

	if (press_stack_ > 0 && is_command_key(event->key()))
		press_stack_--;

	if (press_stack_ == 0)
		repeat_timer_->stop();

	qDebug() << "released: " << event->key()
		<< ", press_stack_: " << press_stack_;

	player_ctrl_ &= ~common_key_prepare(event);
	emit command(player_ctrl_);
}

uint64_t KeyboardController::common_key_prepare(QKeyEvent *event) const {
	switch (event->key()) {
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

