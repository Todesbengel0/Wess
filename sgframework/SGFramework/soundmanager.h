#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "qlist.h"

class QString;
class QMediaPlayer;
class QMediaPlaylist;
class QMediaContent;

//! Erlaubt das einfache Abspielen von Sounds. Es gibt (noch) keinen 3D-Sound. Es ist möglich mittels Aufrufen von  addAmbientSound() eine Playlist für
//! ambienten Sound aufzubauen, diese wird dann im Loop abgespielt.
//! Andere Sounds, die z.B. über Trigger oder bei bestimmten Events abgespielt werden, müssen über registerSound()
//! registriert werden. Danach können Sie über playSound() abgespielt werden. Diese Sounds werden nacheinander abgespielt,
//! während der ambiente Sound durchgehend läuft.
class SoundManager
{
public:
    static SoundManager *instance();
    //! Registrieren einer Sounddatei im Soundmanager. Gibt ein Handle zurück, mit dem der Sound später abgespielt werden kann.
    unsigned int registerSound(QString pFileName);
    //! Spielt registrierten Sound ab.
    //! \param pSoundNr Nummer des registrierten Sounds (Rückgabewert von registerSound())
    void playSound(unsigned int pSoundNr);
    //! Setzt übergebenen Sound als (einzigen) ambienten Sound.
    void setAmbientSoundLoop(QString pFileName);
    //! Fügt pFileName der Playliste der ambienten Sounds hinzu. Über pClear kann gesteuert werden,
    //! ob die Liste zuvor geleert werden soll.
    void addAmbientSoundLoop(QString pFileName, bool pClear=false);
    //! Regelt die Lautstärke der ambienten Sounds.
    //! \param pVolume muss zwischen 0 und 100 liegen. Dies ist jeweils der Prozentsatz
    //! von der eingestellten Lautstärke des Ausgabegeräts.
    void setAmbientVolume(unsigned int pVolume);
    //! Regelt die Lautstärke des ambienten Sounds.
    //! \param pVolume siehe setAmbientVolume()
    void setVolume(unsigned int pVolume);
protected:
    SoundManager();
    static SoundManager *mInstance;
    QMediaPlayer *mPlayer, *mAmbientPlayer;
    QMediaPlaylist *mAmbientPlaylist; //! Einfachste Möglichkeit einen Loop zu bekommen
    unsigned int mNrOfActiveSounds;
    QList<QMediaContent *> mSounds;
};

#endif // SOUNDMANAGER_H
