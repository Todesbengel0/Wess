#include "openalsoundapi.h"
#include "soundsource.h"
#include <qdebug.h>
#include <qlibrary.h>
#include "openalhelper.h"
#define AL_ALEXT_PROTOTYPES 1
#include <efx.h>
#include "efxreverbeffect.h"

OpenALSoundAPI::OpenALSoundAPI()
{
    //alDistanceModel(AL_LINEAR_DISTANCE);
}


void OpenALSoundAPI::play(unsigned int soundsourceid)
{
    ALuint id = static_cast<ALuint>(soundsourceid);
    OpenALHelper::funcalSourcePlay(id);
}

void OpenALSoundAPI::pause(unsigned int soundsourceid)
{
    ALuint id = static_cast<ALuint>(soundsourceid);
    OpenALHelper::funcalSourcePause(id);
}

void OpenALSoundAPI::stop(unsigned int soundsourceid)
{
    ALuint id = static_cast<ALuint>(soundsourceid);
    OpenALHelper::funcalSourceStop(id);
}

void OpenALSoundAPI::setSoundSourcePosition(unsigned int soundsourceid, const QVector3D &newPos)
{
    ALuint id = static_cast<ALuint>(soundsourceid);
    OpenALHelper::funcalSource3f(id, AL_POSITION, newPos.x(), newPos.y(), newPos.z());

}

void OpenALSoundAPI::setListenerPosition(const QVector3D &position)
{
    OpenALHelper::funcalListener3f(AL_POSITION, position.x(), position.y(), position.z());
}

void OpenALSoundAPI::setListenerOrientation(const QVector3D &dir, const QVector3D &up)
{
    float orientation[6];
    orientation[0]=dir.x();
    orientation[1]=dir.y();
    orientation[2]=dir.z();
    orientation[3]=up.x();
    orientation[4]= up.y();
    orientation[5]= up.z();

    OpenALHelper::funcalListenerfv(AL_ORIENTATION, orientation);
}

void OpenALSoundAPI::setListenerVelocity(const QVector3D &velocity)
{
     OpenALHelper::funcalListener3f(AL_VELOCITY, velocity.x(), velocity.y(), velocity.z());
}

unsigned int OpenALSoundAPI::createSoundSource(unsigned int soundbufferid)
{
    ALint soundSourceId;
    OpenALHelper::funcalGenSources(1, &soundSourceId);


    OpenALHelper::funcalSourcei(soundSourceId, AL_BUFFER, soundbufferid);


    return static_cast<unsigned int>(soundSourceId);

}

unsigned int OpenALSoundAPI::createSound(QString const& filepath)
{
    return mSoundFileLoader.loadFile(filepath);
}


void OpenALSoundAPI::setSoundSourcePitch(unsigned int soundsourceid, const float pitch)
{
     ALuint id = static_cast<ALuint>(soundsourceid);
    OpenALHelper::funcalSourcef(id, AL_PITCH, pitch);
}

void OpenALSoundAPI::setSoundSourceVolume(unsigned int soundsourceid, const float volume)
{
    ALuint id = static_cast<ALuint>(soundsourceid);
    OpenALHelper::funcalSourcef(id, AL_GAIN, volume);
}

void OpenALSoundAPI::setSoundSourceLooping(unsigned int soundsourceid, const bool isLooping)
{
    ALuint id = static_cast<ALuint>(soundsourceid);
    OpenALHelper::funcalSourcei(id, AL_LOOPING, isLooping);
}

void OpenALSoundAPI::setSoundSourceVelocity(unsigned int soundsourceid, const QVector3D &velocity)
{
    ALuint id = static_cast<ALuint>(soundsourceid);

    OpenALHelper::funcalSource3f(id, AL_VELOCITY, velocity.x(), velocity.y(), velocity.z());

}

OpenALSoundAPI::~OpenALSoundAPI()
{
    OpenALHelper::funcalalcMakeContextCurrent(NULL);
    OpenALHelper::funcalcDestroyContext(context);
    OpenALHelper::funcalcCloseDevice(device);
}


bool OpenALSoundAPI::init()
{
     if(OpenALHelper::loadDLLAndFunctions())
     {
        ALenum errorNum = OpenALHelper::funcalGetError();

        // initialise OpenAL
        device = OpenALHelper::funcalcOpenDevice(nullptr);

        if(OpenALHelper::funcalIsExtensionPresent(device, (ALchar *) "ALC_EXT_EFX"))
        {
            ALint attribs[4] = { 0 };

            // Use Context creation hint to request 4 Auxiliary
            // Sends per Source
            attribs[0] = ALC_MAX_AUXILIARY_SENDS;
            attribs[1] = 1;
            //XXX End block, but in addition we have set attrib as a parameter in alcCreateContext()

            context = OpenALHelper::funcalcCreateContext(device, attribs);
        }
        else
        {
            context = OpenALHelper::funcalcCreateContext(device, nullptr);
        }


        OpenALHelper::funcalalcMakeContextCurrent(context);
        errorNum = OpenALHelper::funcalGetError();
        if (errorNum != AL_NO_ERROR)
        {
            qDebug()<<"Error initializing OpenAL.";
            return false;
        }
        return true;
     }
     else
        return false;
}


void OpenALSoundAPI::setEffectReverb(unsigned int soundSourceId, EFXEAXREVERBPROPERTIES efxReverbProperty)
{
    EFXReverbEffect *reverbEffect = new EFXReverbEffect((ALuint)soundSourceId, device);


    // Currently unused effects/filters are commented out
    //effect->setEffect(AL_EFFECT_REVERB, cbConfigureEffect); // AL_EFFECT_NULL, AL_EFFECT_REVERB, AL_EFFECT_CHORUS
    //effect->setEffect(AL_EFFECT_REVERB, cbConfigureEffect, AL_FILTER_LOWPASS, 0.5, 0.0, 0.5); //cbConfigureFilter); // AL_FILTER_NULL  ..  AL_EFFECT_NULL, AL_EFFECT_REVERB, AL_EFFECT_CHORUS
    reverbEffect->setReverbEffect(efxReverbProperty, 0.5, 0.5);
}
