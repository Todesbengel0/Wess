#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H
#include <QVector3D>
#include <memory>
#include "audiolistener.h"
#include "idleobserver.h"
#include "efx-presets.h"
class ISoundAPI;
class SoundSource;
class SoundFile;

//!
//! \brief Der AudioEngineType enum beschreibt die Art der SoundAPI
//!
enum class AudioEngineType{
    QtStereo,
    OpenAL3D
};
//!
//! \brief Der ListenerType beschreibt  Berechnungsgrundlage
//!
enum class ListenerType
{
    MANUAL,
    CAMERAPOS
};
//!
//! \brief Managed die verschiedenen SoundAPIs und dient als Schnittstelle zur Nutzung von Audio
//! Delegiert Anfragen an die active SoundAPI.
//!
class AudioEngine : public IdleObserver
{

public:
    static AudioEngine& instance();

    //!
    //! \brief Initialisiert die Audio Engine
    //! \param audioEngineType Beschreibt welche SoundAPI genutzt werden soll
    //! \param listenertype Beschreibt worauf sich die Listenerposition bezieht
    //! \param audiolistener Beschreibt Referenzobject zur Berechnung der Position des SoundListeners
    //!
    void init(AudioEngineType audioEngineType, ListenerType listenertype = ListenerType::CAMERAPOS, AudioListener* audiolistener = nullptr);

    //!
    //! \brief setActiveListener setzt den activen AudioListener
    //! \param listener der neue listener
    //!
    void setActiveListener(AudioListener* listener);

    // IdleObserver interface
    void doIt() override;
private:
    friend class SoundSource;
    friend class SoundFile;

    AudioEngine();
    ~AudioEngine();

    //!
    //! \brief play delegiert play an SoundAPI
    //! \param soundsouceid ID der Soundsource
    //!
    void play(unsigned int soundsouceid);
    //!
    //! \brief pause delegiert pause an SoundAPI
    //! \param soundsouceid ID der Soundsource
    //!
    void pause(unsigned int soundsouceid);
    //!
    //! \brief stop delegiert stop an SoundAPI
    //! \param soundsouceid ID der Soundsource
    //!
    void stop(unsigned int soundsouceid);
    //!
    //! \brief setSoundSourcePosition delegiert setSoundSourcePosition an SoundAPI
    //! \param soundsouceid ID der Soundsource
    //! \param newPos neue Position
    //!
    void setSoundSourcePosition(unsigned int soundsouceid, const QVector3D &newPos);
    //!
    //! \brief setSoundSourcePitch delegiert setSoundSourcePitch an SoundAPI
    //! \param soundsouceid ID der SoundSource
    //! \param pitch neuer Wert für Pitch
    //!
    void setSoundSourcePitch(unsigned int soundsouceid, const float pitch);
    //!
    //! \brief setSoundSourceVolume delegiert setSoundSourceVolume an SoundAPI
    //! \param soundsouceid ID der SoundSource
    //! \param volume neuer Wert für Lautstaerke
    //!
    void setSoundSourceVolume(unsigned int soundsouceid, const float volume);
    //!
    //! \brief setSoundSourceVelocity delegiert setSoundSourceVelocity an SoundAPI
    //! \param soundsouceid ID der SoundSource
    //! \param velocity neuer Wert für Geschwindigkeit
    //!
    void setSoundSourceVelocity(unsigned int soundsouceid, const QVector3D &velocity);
    //!
    //! \brief setSoundSourceLooping delegiert setSoundSourceLooping an SoundAPI
    //! \param soundsouceid ID der SoundSource
    //! \param isLooping Wert für Looping true/false
    //!
    void setSoundSourceLooping(unsigned int soundsouceid, const bool isLooping);

    //!
    //! \brief setEffectReverb delegiert setEffectReverb an SoundAPI
    //! \param soundSourceId ID der SoundSource
    //! \param efxReverbProperty Beschreibt Eigenschaften des Reverbeffects
    //!
    void setEffectReverb(unsigned int soundSourceId, EFXEAXREVERBPROPERTIES efxReverbProperty);

    //!
    //! \brief setListenerPosition delegiert setListenerPosition an SoundAPI
    //! \param position neue Position
    //!
    void setListenerPosition(QVector3D const& position);
    //!
    //! \brief setListenerOrientation delegiert setListenerOrientation an SoundAPI
    //! \param orientation neue orientierung
    //! \param up neuer upvector
    //!
    void setListenerOrientation(QVector3D const& orientation, const QVector3D &up);
    //!
    //! \brief setListenerVelocity delegiert setListenerVelocity an SoundAPI
    //! \param velocity neue Geschwindigkeit
    //!
    void setListenerVelocity(QVector3D const& velocity);

    //!
    //! \brief createSoundSource delegiert createSoundSource an SoundAPI
    //! \param soundbufferid ID des Soundbuffers
    //! \return
    //!
    unsigned int createSoundSource(unsigned int soundbufferid);
    //!
    //! \brief createSound delegiert an SoundAPI
    //! \param filepath Pfad zur Audiodatei(Nur WAVE PCM 16 Bit)
    //! \return
    //!
    unsigned int createSound(QString const& filepath);

    AudioListener* mAudioListener;
    ISoundAPI* mSoundAPI;
    ListenerType mListenerType;
    QVector3D mOldListenerPos;

};

#endif // AUDIOENGINE_H
