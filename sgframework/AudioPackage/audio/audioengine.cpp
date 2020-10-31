#include "audioengine.h"
#include <al.h>
#include <alc.h>
#include <qlibrary.h>
#include <qdebug.h>
#include "soundsource.h"
#include "audiolistener.h"
#include "openalsoundapi.h"
#include "qtstereosoundapi.h"
#include "scenemanager.h"
#include <iostream>

AudioEngine::AudioEngine()
{

}

AudioEngine::~AudioEngine()
{
    if(mSoundAPI)
    {
        delete mSoundAPI;
    }
}

AudioEngine &AudioEngine::instance()
{
    static AudioEngine mInstance;
    return mInstance;
}


void AudioEngine::init(AudioEngineType audioEngineType,ListenerType listenertype, AudioListener* audiolistener)
{
    mListenerType = listenertype;
    mAudioListener= audiolistener;

    switch(audioEngineType)
    {
        case AudioEngineType::OpenAL3D:
        {
                mSoundAPI= new OpenALSoundAPI();
                if(mSoundAPI->init())
                {
                    break;
                }
                qDebug() << "Init OpenAL failed ... trying QtStereo.";
        }
        case AudioEngineType::QtStereo:
        {
            qDebug() << "Init QtStereo...";
            mSoundAPI= new QtStereoSoundAPI();
            break;
        }

    }
}

void AudioEngine::play(unsigned int soundsourceid)
{
    mSoundAPI->play(soundsourceid);
}

void AudioEngine::pause(unsigned int soundsourceid)
{
    mSoundAPI->pause(soundsourceid);
}

void AudioEngine::stop(unsigned int soundsourceid)
{
    mSoundAPI->stop(soundsourceid);
}

void AudioEngine::setSoundSourcePosition(unsigned int soundsourceid, const QVector3D &newPos)
{
    mSoundAPI->setSoundSourcePosition(soundsourceid, newPos);
}

void AudioEngine::setSoundSourcePitch(unsigned int soundsourceid, const float pitch)
{
    mSoundAPI->setSoundSourcePitch(soundsourceid, pitch);
}

void AudioEngine::setSoundSourceVolume(unsigned int soundsourceid, const float volume)
{
    mSoundAPI->setSoundSourceVolume(soundsourceid,volume);
}

void AudioEngine::setSoundSourceLooping(unsigned int soundsourceid, const bool isLooping)
{
    mSoundAPI->setSoundSourceLooping(soundsourceid, isLooping);
}

void AudioEngine::setEffectReverb(unsigned int soundSourceId, EFXEAXREVERBPROPERTIES efxReverbProperty)
{
    mSoundAPI->setEffectReverb(soundSourceId, efxReverbProperty);
}

void AudioEngine::setSoundSourceVelocity(unsigned int soundsourceid, const QVector3D &velocity)
{
    mSoundAPI->setSoundSourceVelocity(soundsourceid, velocity);
}

void AudioEngine::setListenerPosition(const QVector3D & position)
{
    mSoundAPI->setListenerPosition(position);
}

void AudioEngine::setListenerOrientation(const QVector3D & orientation, const QVector3D &up)
{
    mSoundAPI->setListenerOrientation(orientation, up);
}

void AudioEngine::setListenerVelocity(const QVector3D &velocity)
{
    mSoundAPI->setListenerVelocity(velocity);
}

unsigned int AudioEngine::createSoundSource(unsigned int soundbufferid)
{
    return mSoundAPI->createSoundSource(soundbufferid);
}

unsigned int AudioEngine::createSound(QString const& filepath)
{
    return mSoundAPI->createSound(filepath);
}


void AudioEngine::doIt()
{
    QVector3D pos;
    QVector3D dir, up;
    switch(mListenerType)
    {
        case ListenerType::CAMERAPOS:
        {
            Camera cam = *SceneManager::instance()->getActiveContext()->getCamera();
            pos = cam.getPosition();
            dir = cam.getViewDir();
            up= cam.getUpDir();

            break;
        }
        case ListenerType::MANUAL:
        {
            pos = mAudioListener->getPosition();
            dir= mAudioListener->getOrientation().dir;
            up = mAudioListener->getOrientation().up;
            break;
        }
    }
    this->instance().setListenerPosition(pos);


    QVector3D delta= pos - mOldListenerPos;

    mOldListenerPos = pos;


    this->instance().setListenerPosition(pos);
    this->instance().setListenerVelocity(delta);
    this->instance().setListenerOrientation(dir, up);
}
