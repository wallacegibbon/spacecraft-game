#include "MyMediaPlayer.h"

MyMediaPlayer::MyMediaPlayer(QString media_url, QObject *parent) : QMediaPlayer(parent, QMediaPlayer::LowLatency)
{
    setMedia(QUrl(media_url));
}

void MyMediaPlayer::play()
{
    if (state() == QMediaPlayer::PlayingState)
    {
        setPosition(0);
    }
    else if (state() == QMediaPlayer::StoppedState)
    {
        QMediaPlayer::play();
    }
}