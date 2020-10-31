#ifndef SOUNDFILE_H
#define SOUNDFILE_H
#include <QString>

using namespace std;

class SoundFile
{
public:
    //!
    //! \brief Konstruktor für SoundFile
    //! \param path Pfad zur Audiodatei(Nur WAV PCM 16 bit
    //!
    SoundFile(const QString &path);
    //!
    //! \brief getId
    //! \return soundbufferid
    //!
    unsigned int getId() const;
    //!
    //! \brief getPath
    //! \return Pfad zur Audiodatei
    //!
    const QString& getPath() const;

private:
    unsigned int mId;
    QString mPath;
};

#endif // SOUNDFILE_H
