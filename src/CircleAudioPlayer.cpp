#include "CircleAudioPlayer.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>

CircleAudioPlayer::CircleAudioPlayer(QString media_url, QObject *parent)
	: QMediaPlayer(parent)
{
	playlist = new QMediaPlaylist(parent);
	playlist->addMedia(QUrl(media_url));
	playlist->setPlaybackMode(QMediaPlaylist::Loop);

	setPlaylist(playlist);
}
