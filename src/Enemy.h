#ifndef __ENEMY_H
#define __ENEMY_H

#include <QGraphicsPixmapItem>

class Airplane;

class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Enemy(QString _image, int _layer);
    void move();
    int get_score() { return score; }
    int get_layer() { return layer; }
    void handle_bullet_hit(Airplane *player);
    qreal height() { return boundingRect().height(); }
    qreal width() { return boundingRect().width(); }

signals:
    void hit_by_bullet(Airplane *player);

private:
    QString image;
    int y_step;
    int score;
    int layer;
};

class Enemy_0 : public Enemy
{
public:
    Enemy_0(int layer) : Enemy(":/image/enemy_0.png", layer) {}
};

class Enemy_1 : public Enemy
{
public:
    Enemy_1(int layer) : Enemy(":/image/enemy_1.png", layer) {}
};

#endif