#ifndef QTSTEREOSOUNDAPI_H
#define QTSTEREOSOUNDAPI_H
#include "isoundapi.h"
#include <memory>
#include <qlist.h>
#include <qstring.h>

class QMediaPlayer;
class QMediaPlaylist;
class QMediaContent;

class QtStereoSoundAPI : public ISoundAPI
{
public:
    QtStereoSoundAPI();
    ~QtStereoSoundAPI();

    // ISoundAPI interface
public:
    void play(unsigned int soundsourceid) override;
    void pause(unsigned int soundsourceid) override;
    void stop(unsigned int soundsourceid) override;
    void setSoundSourcePosition(unsigned int soundsourceid, const QVector3D &newPos) override;
    void setListenerPosition(const QVector3D &position) override;
    void setListenerOrientation(const QVector3D &orientation, const QVector3D &up) override;
    void setListenerVelocity(const QVector3D &velocity) override;
    unsigned int createSoundSource(unsigned int soundbufferid) override;
    unsigned int createSound(QString const& filepath) override;
    void setSoundSourcePitch(unsigned int soundsourceid, const float pitch) override;
    void setSoundSourceVolume(unsigned int soundsourceid, const float volume) override;
    void setSoundSourceLooping(unsigned int soundsourceid, const bool isLooping) override;
    void setSoundSourceVelocity(unsigned int soundsourceid, const QVector3D &velocity) override;

    bool init() override;
private:
    //QMediaPlayer* mPlayer;
    /*std::unique_ptr<QMediaPlayer> mPlayer;
    std::unique_ptr<QMediaPlayer> mAmbientPlayer;
   std::unique_ptr<QMediaPlaylist> mAmbientPlaylist; //! Einfachste Möglichkeit einen Loop zu bekommen
    unsigned int mNrOfActiveSounds;*/
    //std::vector<std::unique_ptr<QMediaContent>> mSounds;
     //QList<QMediaContent *> mSounds;

     QMediaPlayer *mPlayer, *mAmbientPlayer;
     QMediaPlaylist *mAmbientPlaylist; //! Einfachste Möglichkeit einen Loop zu bekommen
     unsigned int mNrOfActiveSounds;
     QList<QMediaContent *> mSounds;


     // ISoundAPI interface
public:
     void setEffectReverb(unsigned int soundSourceId, EFXEAXREVERBPROPERTIES efxReverProperty) override;
};

#endif // QTSTEREOSOUNDAPI_H
