#ifndef __MY_MEDIA_PLAYER_H
#define __MY_MEDIA_PLAYER_H

#include <QMediaPlayer>

class MyMediaPlayer : public QMediaPlayer
{
public:
    MyMediaPlayer(QObject *parent = nullptr) : QMediaPlayer(parent, QMediaPlayer::LowLatency) {}
    MyMediaPlayer(QString media_url, QObject *parent = nullptr);
    void play();
};

#endif