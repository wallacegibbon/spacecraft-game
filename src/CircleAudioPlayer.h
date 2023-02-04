#pragma once

#include <QMediaPlayer>
#include <QMediaPlaylist>

class CircleAudioPlayer : public QMediaPlayer {
  QMediaPlaylist *playlist_;

public:
  CircleAudioPlayer(QString media_url, QObject *parent = nullptr);
};
