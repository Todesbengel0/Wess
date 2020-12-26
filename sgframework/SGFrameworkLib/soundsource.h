#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H
#include <vector>
#include "efx-presets.h"
#include "soundfile.h"
#include "sgpositionalobject.h"

class SoundSource : public SGPositionalObject
{
public:
    //!
    //! \brief SoundSource Konstruktor für SoundSource
    //! \param soundfile Instanz eines SoundFiles
    //! \param volume Lautstärke
    //! \param isLooping Wiederholung des Sounds ja/nein
    //!
    SoundSource(SoundFile* soundfile, float volume=100, bool isLooping=false);
    //!
    //! \brief play startet Wiedergabe des Sounds
    //!
    void play();
    //!
    //! \brief pause pausiert Wiedergabe
    //!
    void pause();
    //!
    //! \brief stop stopped wiedergabe
    //!
    void stop();
    //!
    //! \brief getSoundFile
    //! \return Soundfileinstanz
    //!
    SoundFile* getSoundFile();

    //!
    //! \brief getId
    //! \return soundsourceID
    //!
    unsigned int getId()const;

    //!
    //! \brief setPitch setzt Pitch
    //! \param pitch neuer Pitch
    //!
    void setPitch(float pitch);
    //!
    //! \brief setVolume setzt Lautstärke
    //! \param volume neue Lautstärke
    //!
    void setVolume(float volume);
    //!
    //! \brief setLooping setzt Wiederholung des Sounds
    //! \param isLooping Wiederholung des Sounds ja/nein
    //!
    void setLooping(bool isLooping);

    //!
    //! \brief setVelocity setzt Geschwindigkeit
    //! \param velocity neue Geschwindikeit
    //!
    void setVelocity(QVector3D const& velocity);
    //!
    //! \brief getVelocity
    //! \return aktuelle Geschwindigkeit
    //!
    const QVector3D& getVelocity() const;
    //!
    //! \brief setReverbEffect setzt ReverbEffect
    //! \param efxReverbProperty Eigenschaften des Reverbeffects
    //!
    void setReverbEffect(EFXEAXREVERBPROPERTIES efxReverbProperty);

private:
    SoundFile* mSoundFile;
    unsigned int mId;
    QVector3D mVelocity;

    QVector3D mPosition;
    // SGObject interface
protected:
    virtual void beforeTraverseChilds(SGObjectVisitor &visitor) override;
};

#endif // SOUNDSOURCE_H
