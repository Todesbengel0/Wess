#include "qtstereosoundapi.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QList>
#include <QFileInfo>
#include "soundsource.h"
#include "soundmanager.h"

QtStereoSoundAPI::QtStereoSoundAPI()
{
    mPlayer = new QMediaPlayer();
    mAmbientPlayer = new QMediaPlayer();
    mAmbientPlaylist = new QMediaPlaylist();
    mAmbientPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    mAmbientPlayer->setPlaylist(mAmbientPlaylist);
    mNrOfActiveSounds = 0;
}

QtStereoSoundAPI::~QtStereoSoundAPI()
{
}

void QtStereoSoundAPI::play(unsigned int soundsourceid)
{
    mPlayer->setMedia(*(mSounds[soundsourceid-1]));
    mPlayer->play();
}

void QtStereoSoundAPI::pause(unsigned int soundsourceid)
{
    Q_UNUSED(soundsourceid);
}

void QtStereoSoundAPI::stop(unsigned int soundsourceid)
{
    Q_UNUSED(soundsourceid);
}

void QtStereoSoundAPI::setSoundSourcePosition(unsigned int soundsourceid, const QVector3D &newPos)
{
    Q_UNUSED(soundsourceid);
     Q_UNUSED(newPos);
}

void QtStereoSoundAPI::setListenerPosition(const QVector3D &position)
{
    Q_UNUSED(position);
}

void QtStereoSoundAPI::setListenerOrientation(const QVector3D &orientation, const QVector3D &up)
{
    Q_UNUSED(orientation);
}

void QtStereoSoundAPI::setListenerVelocity(const QVector3D &velocity)
{
    Q_UNUSED(velocity);
}

unsigned int QtStereoSoundAPI::createSoundSource(unsigned int soundbufferid)
{
    return soundbufferid;
}

unsigned int QtStereoSoundAPI::createSound(QString const& filepath)
{
    QFileInfo lFileInfo(filepath);
    if (lFileInfo.exists())
    {
        QUrl url = QUrl::fromLocalFile(lFileInfo.absoluteFilePath());
        QMediaContent *lContent = new QMediaContent(url);

        if (!lContent->isNull())
        {
            mSounds.push_back(lContent);
            return(mSounds.length());
        }
        return(0);
    }
    else
    {
        qDebug() << "Soundmanager: " << filepath << " not found.";
        return(0);
    }
}


void QtStereoSoundAPI::setSoundSourcePitch(unsigned int soundsourceid, const float pitch)
{
    Q_UNUSED(soundsourceid);
    Q_UNUSED(pitch);
}

void QtStereoSoundAPI::setSoundSourceVolume(unsigned int soundsourceid, const float volume)
{
    Q_UNUSED(soundsourceid);
    mPlayer->setVolume(volume);
    mAmbientPlayer->setVolume(volume);
}

void QtStereoSoundAPI::setSoundSourceLooping(unsigned int soundsourceid, const bool isLooping)
{
    if(isLooping)
    {
            mAmbientPlaylist->addMedia(*mSounds[soundsourceid-1]);
            if (mAmbientPlayer->state() != QMediaPlayer::PlayingState)
                mAmbientPlayer->play();
    }
}

void QtStereoSoundAPI::setSoundSourceVelocity(unsigned int soundsourceid, const QVector3D &velocity)
{
}


bool QtStereoSoundAPI::init()
{
    return(true);
}



void QtStereoSoundAPI::setEffectReverb(unsigned int soundSourceId, EFXEAXREVERBPROPERTIES efxReverProperty)
{
}
