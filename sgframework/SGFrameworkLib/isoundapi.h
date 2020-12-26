#ifndef ISOUNDAPI_H
#define ISOUNDAPI_H
#include <QVector3D>
#include "efx-presets.h"

class SoundSource;
class SoundFile;
class ISoundAPI
{
public:
    ISoundAPI();
    virtual ~ISoundAPI();

    //!
    //! \brief init initialisieren der SoundAPI
    //! \return Angabe über Erfolg oder Misserfolg
    //!
    virtual bool init()=0;
    //!
    //! \brief play startet die Wiedergabe einer SoundSource
    //! \param soundsourceid ID der SoundSource
    //!
    virtual void play(unsigned int)=0;
    //!
    //! \brief pause pausiert die Wiedergabe einer SoundSource
    //! \param soundsourceid ID der SoundSource
    //!
    virtual void pause(unsigned int)=0;
    //!
    //! \brief stop stopped die Wiedergabe einer SoundSource
    //! \param soundsourceid ID der SoundSource
    //!
    virtual void stop(unsigned int)=0;
    //!
    //! \brief setSoundSourcePosition setzt Position der SoundSource
    //! \param soundsourceid ID der SoundSource
    //! \param newPos neue Position
    //!
    virtual void setSoundSourcePosition(unsigned int soundsourceid, const QVector3D &newPos)=0;
    //!
    //! \brief setSoundSourcePitch setzt Pitch der SoundSource
    //! \param soundsourceid ID der SoundSource
    //! \param pitch neuer Pitch
    //!
    virtual void setSoundSourcePitch(unsigned int soundsourceid, const float pitch)=0;
    //!
    //! \brief setSoundSourceVolume setzt Lautstärke der SoundSource
    //! \param soundsourceid ID der SoundSource
    //! \param volume neue Lautstärke
    //!
    virtual void setSoundSourceVolume(unsigned int soundsourceid, const float volume)=0;
    //!
    //! \brief setSoundSourceLooping setzt Looping
    //! \param soundsourceid ID der SoundSource
    //! \param isLooping beschreibt ob SoundSource den Sound wiederholen soll
    //!
    virtual void setSoundSourceLooping(unsigned int soundsourceid, const bool isLooping)=0;
    //!
    //! \brief setSoundSourceVelocity setzt Geschwindigkeit der SoundSource
    //! \param soundsourceid ID der SoundSource
    //! \param velocity neue Geschwindigkeit
    //!
    virtual void setSoundSourceVelocity(unsigned int soundsourceid, const QVector3D &velocity)=0;
    //!
    //! \brief setListenerPosition setzt Position des Listeners
    //! \param position
    //!
    virtual void setListenerPosition(QVector3D const& position)=0;
    //!
    //! \brief setListenerOrientation setzt Orientierung des Listeners
    //! \param orientation neuer Richtungsvektor
    //! \param up neuer Up Vektor
    //!
    virtual void setListenerOrientation(QVector3D const& orientation, const QVector3D &up)=0;
    //!
    //! \brief setListenerVelocity setzt Geschwindigkeit des Listeners
    //! \param velocity neue Geschwindigkeit
    //!
    virtual void setListenerVelocity(QVector3D const& velocity)=0;
    //!
    //! \brief createSoundSource erstellt neue SoundSource
    //! \param soundbufferid ID des Soundbuffers
    //! \return
    //!
    virtual unsigned int createSoundSource(unsigned int soundbufferid)=0;
    //!
    //! \brief createSound erstellt neuen Sound
    //! \param filePath Pfad zur Audiodatei(Nur WAV PCM 16 Bit)
    //! \return
    //!
    virtual unsigned int createSound(QString const& filePath)=0;
    //!
    //! \brief setEffectReverb setzt ReverbEffect
    //! \param soundSourceId ID der SoundSource
    //! \param efxReverProperty Eigenschaften des Reverbeffekts
    //!
    virtual void setEffectReverb(unsigned int soundSourceId, EFXEAXREVERBPROPERTIES efxReverProperty)=0;

};

#endif // ISOUNDAPI_H
