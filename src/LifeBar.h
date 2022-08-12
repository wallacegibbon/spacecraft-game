#ifndef __LIFE_BAR_H
#define __LIFE_BAR_H

#include <QGraphicsRectItem>

class LifeBar : public QGraphicsRectItem {
public:
	LifeBar(int _init_value, QGraphicsItem *parent = nullptr);
	void update_value(int value);
	int height() const { return rect().height(); }
	int width() const { return rect().width(); }

private:
	QGraphicsRectItem *value_bar;
	int init_value;
	int current_value;
	int bar_width = 30;
	int bar_height = 5;
};

#endif
