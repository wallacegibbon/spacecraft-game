#include "CircleAudioPlayer.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>

CircleAudioPlayer::CircleAudioPlayer(QString media_url, QObject *parent)
	: QMediaPlayer(parent) {

	playlist_ = new QMediaPlaylist(parent);
	playlist_->addMedia(QUrl(media_url));
	playlist_->setPlaybackMode(QMediaPlaylist::Loop);

	setPlaylist(playlist_);
}
