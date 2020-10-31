#include "soundsource.h"
#include "audioengine.h"
#include "../SGFramework/sgobjectvisitor.h"
//!
//! \brief SoundSource::SoundSource
//! \param soundfile
//! \param volume
//! \param isLooping
//!
SoundSource::SoundSource(SoundFile *soundfile, float volume, bool isLooping)
{
   mSoundFile=soundfile;

   mId = AudioEngine::instance().createSoundSource(mSoundFile->getId());

   this->setVolume(volume);

   this->setLooping(isLooping);

}

unsigned int SoundSource::getId() const
{
    return mId;
}

void SoundSource::setPitch(float pitch)
{
    AudioEngine::instance().setSoundSourcePitch(mId, pitch);
}

void SoundSource::setVolume(float volume)
{
    AudioEngine::instance().setSoundSourceVolume(mId, volume);
}

void SoundSource::setLooping(bool isLooping)
{
    AudioEngine::instance().setSoundSourceLooping(mId, isLooping);
}

void SoundSource::setVelocity(const QVector3D &velocity)
{
    mVelocity = velocity;
}

const QVector3D &SoundSource::getVelocity() const
{
    return mVelocity;
}

void SoundSource::setReverbEffect(EFXEAXREVERBPROPERTIES efxReverbProperty)
{
    AudioEngine::instance().setEffectReverb(mId, efxReverbProperty);
}

void SoundSource::play()
{
    AudioEngine::instance().play(mId);
}

void SoundSource::pause()
{
    AudioEngine::instance().pause(mId);
}

void SoundSource::stop()
{
    AudioEngine::instance().stop(mId);
}

SoundFile *SoundSource::getSoundFile()
{
    return mSoundFile;
}

void SoundSource::beforeTraverseChilds(SGObjectVisitor &visitor)
{
    SGPositionalObject::beforeTraverseChilds(visitor);

    QVector3D oldPos = mPosition;

    mPosition= mWorldMatrix * QVector3D(0,0,0);

    QVector3D delta= mPosition - oldPos;

    mVelocity = delta;

    AudioEngine::instance().setSoundSourcePosition(mId, QVector3D(mPosition.x(), mPosition.y(), mPosition.z()));
    AudioEngine::instance().setSoundSourceVelocity(mId, QVector3D(mVelocity.x(), mVelocity.y(), mVelocity.z()));

}
