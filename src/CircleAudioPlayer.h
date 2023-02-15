#pragma once

#include <QMediaPlayer>
#include <QMediaPlaylist>

class CircleAudioPlayer : public QMediaPlayer {
public:
	CircleAudioPlayer(QString media_url, QObject *parent = nullptr);

private:
	QMediaPlaylist *playlist_;
};
