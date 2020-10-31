#include "soundfile.h"
#include "audioengine.h"

SoundFile::SoundFile(const QString& path)
{
    mPath = path;
    mId = AudioEngine::instance().createSound(mPath);
}

unsigned int SoundFile::getId() const
{
    return mId;
}

const QString &SoundFile::getPath() const
{
    return mPath;
}
