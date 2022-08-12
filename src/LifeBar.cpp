#include "LifeBar.h"
#include <QBrush>
#include <QPen>

LifeBar::LifeBar(int _init_value, QGraphicsItem *parent)
	: init_value(_init_value), QGraphicsRectItem(parent)
{
	setRect(0, 0, bar_width, bar_height);
	setBrush(QBrush(Qt::darkGray));
	setPen(QPen(Qt::NoPen));
	value_bar = new QGraphicsRectItem(this);
	value_bar->setBrush(QBrush(Qt::darkRed));
	value_bar->setPen(QPen(Qt::NoPen));
	update_value(init_value);
}

void LifeBar::update_value(int value) {
	current_value = value > init_value ? init_value : value;
	int current_width =
		bar_width * (static_cast<double>(current_value) / init_value);

	value_bar->setRect(0, 0, current_width, bar_height);
}
