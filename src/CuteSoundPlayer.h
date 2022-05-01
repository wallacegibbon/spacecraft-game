#ifndef __MY_MEDIA_PLAYER_H
#define __MY_MEDIA_PLAYER_H

#include <QMediaPlayer>

class CuteSoundPlayer : public QMediaPlayer {
public:
    CuteSoundPlayer(QObject *parent = nullptr) : QMediaPlayer(parent, QMediaPlayer::LowLatency) {}
    CuteSoundPlayer(QString media_url, QObject *parent = nullptr);
    void play();
};

#endif