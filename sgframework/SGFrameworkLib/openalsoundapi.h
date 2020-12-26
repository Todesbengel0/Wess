#ifndef OPENALSOUNDAPI_H
#define OPENALSOUNDAPI_H
#include "isoundapi.h"
#include "al.h"
#include "alc.h"
#include "soundfileloader.h"
#include "qlibrary.h"



class OpenALSoundAPI: public ISoundAPI
{
public:
    OpenALSoundAPI();
    ~OpenALSoundAPI();

    // ISoundAPI interface
public:

    bool init() override;

    void play(unsigned int soundsourceid) override;
    void pause(unsigned int soundsourceid) override;
    void stop(unsigned int soundsourceid) override;
    void setSoundSourcePosition(unsigned int soundsourceid, const QVector3D &newPos) override;
    void setListenerPosition(const QVector3D &position) override;
    void setListenerOrientation(const QVector3D &orientation, const QVector3D &up) override;
    void setListenerVelocity(const QVector3D &velocity) override;
    unsigned int createSoundSource(unsigned int soundsourceid) override;
    unsigned int createSound(QString const& filepath) override;
    void setSoundSourcePitch(unsigned int soundsourceid, const float pitch) override;
    void setSoundSourceVolume(unsigned int soundsourceid, const float volume) override;
    void setSoundSourceLooping(unsigned int soundsourceid, const bool isLooping) override;
    void setSoundSourceVelocity(unsigned int soundsourceid, const QVector3D &velocity) override;
    void setEffectReverb(unsigned int soundSourceId, EFXEAXREVERBPROPERTIES efxReverProperty) override;

private:
     SoundFileLoader mSoundFileLoader;

     ALCdevice* device;
     ALCcontext* context;

};

#endif // OPENALSOUNDAPI_H
