#include "soundmanager.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QList>
#include <QFileInfo>

SoundManager *SoundManager::mInstance = NULL;

SoundManager *SoundManager::instance()
{
    if(mInstance == NULL)
    {
        mInstance = new SoundManager;
        mInstance->mPlayer = new QMediaPlayer();
        mInstance->mAmbientPlayer = new QMediaPlayer();
        mInstance->mAmbientPlaylist = new QMediaPlaylist();
        mInstance->mAmbientPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
        mInstance->mAmbientPlayer->setPlaylist(mInstance->mAmbientPlaylist);
        mInstance->mNrOfActiveSounds = 0;
    }
    return mInstance;
}

//! Sound laden und registrieren. Gibt im Fehlerfall 0 zurück, sonst Nr. des Sounds
unsigned int SoundManager::registerSound(QString pFileName)
{
    QFileInfo lFileInfo(pFileName);
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
        qDebug() << "Soundmanager: " << pFileName << " not found.";
        return(0);
    }

}

void SoundManager::playSound(unsigned int pSoundNr)
{
    mPlayer->setMedia(*(mSounds[pSoundNr-1]));
    mPlayer->play();
}

void SoundManager::setAmbientSoundLoop(QString pFileName)
{
    instance()->addAmbientSoundLoop(pFileName, true);
}

void SoundManager::addAmbientSoundLoop(QString pFileName, bool pClear)
{
    QFileInfo lFileInfo(pFileName);
    if (lFileInfo.exists())
    {
        QUrl url = QUrl::fromLocalFile(lFileInfo.absoluteFilePath());

        if (pClear)
            mAmbientPlaylist->clear();

        mAmbientPlaylist->addMedia(url);
        if (mAmbientPlayer->state() != QMediaPlayer::PlayingState)
            mAmbientPlayer->play();
    }
}

void SoundManager::setAmbientVolume(unsigned int pVolume)
{
    mAmbientPlayer->setVolume(pVolume);
}

void SoundManager::setVolume(unsigned int pVolume)
{
    mPlayer->setVolume(pVolume);
}

SoundManager::SoundManager()
{

}

