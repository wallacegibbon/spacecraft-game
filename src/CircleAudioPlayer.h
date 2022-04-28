#ifndef __CIRCLE_AUDIO_PLAYER_H
#define __CIRCLE_AUDIO_PLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

class CircleAudioPlayer : public QMediaPlayer
{
public:
    CircleAudioPlayer(QString media_url, QObject *parent = nullptr);

private:
    QMediaPlaylist *playlist;
};

#endif