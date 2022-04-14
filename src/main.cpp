#include <QApplication>
#include <QGraphicsScene>
#include "Airplane.h"
#include <QGraphicsView>
#include <QTimer>

int main(int argc, const char *argv[])
{
    QApplication app(argc, const_cast<char **>(argv));

    QGraphicsScene *scene = new QGraphicsScene();
    Airplane *player = new Airplane();
    player->setRect(0, 0, 40, 60);

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

    scene->addItem(player);

    QGraphicsView *view = new QGraphicsView(scene);
    // view->setScene(scene);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->setFixedSize(800, 600);
    scene->setSceneRect(0, 0, 800, 600);

    player->setPos(view->width() / 2 - player->rect().width() / 2, view->height() - player->rect().height() - 10);

    QTimer *timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, player, &Airplane::spawn);

    timer->start(2000);

    // view->showFullScreen();
    view->show();

    return app.exec();
}