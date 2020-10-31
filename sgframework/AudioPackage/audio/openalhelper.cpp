#include "openalhelper.h"

#if defined(Q_OS_MACXXX)
 ALCdevice* (*OpenALHelper::funcalcOpenDevice)(const ALchar*);
 ALCcontext* (*OpenALHelper::funcalcCreateContext)(ALCdevice*, const ALCint*);
 ALCboolean (*OpenALHelper::funcalalcMakeContextCurrent)(ALCcontext*);
 ALenum(*OpenALHelper::funcalGetError)();
 ALCboolean (*OpenALHelper::funcalcCloseDevice)(ALCdevice*);
 void (*OpenALHelper::funcalSourcePlay)(ALuint);
 void (*OpenALHelper::funcalSourcePause)(ALuint);
 void (*OpenALHelper::funcalSourceStop)(ALuint);
 void (*OpenALHelper::funcalListener3f)(ALenum, ALfloat, ALfloat, ALfloat);
 void (*OpenALHelper::funcalSource3f)(ALuint, ALenum, ALfloat, ALfloat, ALfloat);
  void (*OpenALHelper::funcalSourcei)(ALuint, ALenum, ALint);
  void (*OpenALHelper::funcalSourcef)(ALuint, ALenum, ALfloat);
  void (*OpenALHelper::funcalGenSources)(ALsizei, ALuint*);
  void (*OpenALHelper::funcalcDestroyContext)(ALCcontext*);
  void (*OpenALHelper::funcalBufferData) (ALuint, ALenum, const ALvoid*, ALsizei, ALsizei);
 void(*OpenALHelper::funcalGenBuffers)(ALsizei, ALuint*);
 void (*OpenALHelper::funcalListenerfv)(ALenum, const ALfloat*);
 ALboolean (*OpenALHelper::funcalIsExtensionPresent)(ALCdevice*, const ALchar*);
 void (*OpenALHelper::funcalSource3i)(ALuint, ALenum, ALint, ALint, ALint);

//EFX functions
 void (*OpenALHelper::funcalcGetIntegerv)(ALCdevice*, ALCenum, ALCsizei, ALCint*);
 ALenum (*OpenALHelper::funcalGetEnumValue)(const ALchar*);
// void (*OpenALHelper::funcalGenAuxiliaryEffectSlots)(ALsizei, ALuint*);
// void(*OpenALHelper::funcalGenEffects)(ALsizei, ALuint*);
// void (*OpenALHelper::funcalAuxiliaryEffectSloti)(ALuint, ALenum, ALint);
// void (*OpenALHelper::funcalDeleteEffects)(ALsizei, const ALuint*);
// void (*OpenALHelper::funcalDeleteAuxiliaryEffectSlots)(ALsizei, const ALuint*);
// void (*OpenALHelper::funcalGenFilters)(ALsizei, ALuint*);
// ALboolean (*OpenALHelper::funcalIsFilter)(ALuint);
// void (*OpenALHelper::funcalFilteri)(ALuint, ALenum, ALint );
// void (*OpenALHelper::funcalFilterf)(ALuint, ALenum, ALfloat );
// void (*OpenALHelper::funcalDeleteFilters)(ALsizei, const ALuint*);
// void (*OpenALHelper::funcalEffecti)(ALuint, ALenum, ALint);
// void (*OpenALHelper::funcalEffectf)(ALuint, ALenum, ALfloat);
// void (*OpenALHelper::funcalEffectfv)(ALuint, ALenum, const ALfloat*);
// ALboolean (*OpenALHelper::funcalIsEffect)(ALuint);

#else

//Für Windows müssen die AL-Funktionen aus der dll gebunden werden
#include "qlibrary.h"

ALCdevice* (*OpenALHelper::funcalcOpenDevice)(ALchar*);
ALCcontext* (*OpenALHelper::funcalcCreateContext)(ALCdevice*, const ALCint*);
ALCcontext* (*OpenALHelper::funcalalcMakeContextCurrent)(ALCcontext*);
ALenum(*OpenALHelper::funcalGetError)();
void (*OpenALHelper::funcalcCloseDevice)(ALCdevice*);
void (*OpenALHelper::funcalSourcePlay)(ALuint);
void (*OpenALHelper::funcalSourcePause)(ALuint);
void (*OpenALHelper::funcalSourceStop)(ALuint);
void (*OpenALHelper::funcalListener3f)(ALenum, ALfloat, ALfloat, ALfloat);
void (*OpenALHelper::funcalSource3f)(ALuint, ALenum, ALfloat, ALfloat, ALfloat);
void (*OpenALHelper::funcalSourcei)(ALuint, ALenum, ALint);
void (*OpenALHelper::funcalSourcef)(ALuint, ALenum, ALfloat);
void (*OpenALHelper::funcalGenSources)(ALsizei, ALint*);
ALCcontext* (*OpenALHelper::funcalcDestroyContext)(ALCcontext*);
void (*OpenALHelper::funcalBufferData) (ALuint, ALenum, ALvoid*, ALsizei, ALsizei);
void(*OpenALHelper::funcalGenBuffers)(ALsizei, ALuint*);
void (*OpenALHelper::funcalListenerfv)(ALenum, ALfloat*);
bool (*OpenALHelper::funcalIsExtensionPresent)(ALCdevice*, ALchar*);
void (*OpenALHelper::funcalSource3i)(ALuint, ALenum, ALint, ALint, ALint);

//EFX
void (*OpenALHelper::funcalcGetIntegerv)(ALCdevice*, ALCenum, ALCsizei, ALCint*);
ALenum (*OpenALHelper::funcalGetEnumValue)(ALchar*);

void (*OpenALHelper::funcalGenAuxiliaryEffectSlots)(ALsizei, ALuint*);

void(*OpenALHelper::funcalGenEffects)(ALsizei, ALuint*);

void (*OpenALHelper::funcalAuxiliaryEffectSloti)(ALuint, ALenum, ALint);

void (*OpenALHelper::funcalDeleteEffects)(ALsizei, ALuint*);

void (*OpenALHelper::funcalDeleteAuxiliaryEffectSlots)(ALsizei, ALuint*);

void (*OpenALHelper::funcalGenFilters)(ALsizei, ALuint*);

bool (*OpenALHelper::funcalIsFilter)(ALuint);

void (*OpenALHelper::funcalFilteri)(ALuint, ALenum, ALint );

void (*OpenALHelper::funcalFilterf)(ALuint, ALenum, ALfloat );

void (*OpenALHelper::funcalDeleteFilters)(ALsizei, ALuint*);

void (*OpenALHelper::funcalEffecti)(ALuint, ALenum, ALuint);

bool (*OpenALHelper::funcalIsEffect)(ALuint);

void (*OpenALHelper::funcalEffectf)(ALuint, ALenum, ALfloat);

void (*OpenALHelper::funcalEffectfv)(ALuint, ALenum, const ALfloat*);

#endif

bool OpenALHelper::loadDLLAndFunctions()
{
    QString lLibDir(LIBDIR);
#if defined(Q_OS_MAC)
    QLibrary qLibrary("libopenal.dylib");
#else
    QLibrary qLibrary(lLibDir.append("OpenAL32.dll"));
    qDebug() << "Looking for OpenAL32.dll in " << lLibDir;
#endif

#if defined(Q_OS_MACXXX)
    qDebug() << "OSX Settings for OpenAL.";
    funcalcOpenDevice  = alcOpenDevice;
    funcalcCreateContext  = alcCreateContext;
    funcalalcMakeContextCurrent  = alcMakeContextCurrent;
    funcalGetError = alGetError;
    funcalcCloseDevice = alcCloseDevice;
    funcalcDestroyContext = alcDestroyContext;

    funcalListener3f = alListener3f;
    funcalListenerfv = alListenerfv;


    funcalSourcePlay  = alSourcePlay;
    funcalSourcePause  = alSourcePause;
    funcalSourceStop = alSourceStop;

    funcalSource3f  = alSource3f;
    funcalSource3i  = alSource3i;
    funcalSourcei  = alSourcei;
    funcalSourcef  = alSourcef;

    funcalGenSources  = alGenSources;
    funcalBufferData = alBufferData;
    funcalGenBuffers= alGenBuffers;
    funcalIsExtensionPresent = alcIsExtensionPresent;


    //EFX

    funcalcGetIntegerv = alcGetIntegerv;
    funcalGetEnumValue = alGetEnumValue;
//    funcalGenAuxiliaryEffectSlots = alGenAuxiliaryEffectSlots;
//    funcalGenEffects = alGenEffects;
//    funcalAuxiliaryEffectSloti = alAuxiliaryEffectSloti;
//    funcalDeleteEffects = alDeleteEffects;
//    funcalDeleteAuxiliaryEffectSlots = alDeleteAuxiliaryEffectSlots;
//    funcalGenFilters = alGenFilters;
//    funcalIsFilter = alIsFilter;
//    funcalFilteri = alFilteri;
//    funcalFilterf = alFilterf;
//    funcalDeleteFilters= alDeleteFilters;
//    funcalEffecti = alEffecti;
//    funcalEffectf = alEffectf;
//    funcalEffectfv = alEffectfv;
//    funcalIsEffect = alIsEffect;

    qDebug() << alGetError();
    return(true);
#else

    if(qLibrary.load())
    {

        try{

            funcalcOpenDevice  = (ALCdevice* (*)(ALchar*))qLibrary.resolve("alcOpenDevice");
            funcalcCreateContext  = (ALCcontext* (*)(ALCdevice*, const ALCint*))qLibrary.resolve("alcCreateContext");
            funcalalcMakeContextCurrent  = (ALCcontext* (*)(ALCcontext*))qLibrary.resolve("alcMakeContextCurrent");
            funcalGetError = (ALenum(*)()) qLibrary.resolve("alGetError");
            funcalcCloseDevice = (void (*)(ALCdevice*)) qLibrary.resolve("alcCloseDevice");
            funcalcDestroyContext=(ALCcontext* (*)(ALCcontext*))qLibrary.resolve("alcDestroyContext");

            funcalListener3f = (void (*)(ALenum, ALfloat, ALfloat, ALfloat))qLibrary.resolve("alListener3f");
            funcalListenerfv = (void (*)(ALenum, ALfloat*))qLibrary.resolve("alListenerfv");


            funcalSourcePlay  = (void (*)(ALuint))qLibrary.resolve("alSourcePlay");
            funcalSourcePause  = (void (*)(ALuint))qLibrary.resolve("alSourcePause");
            funcalSourceStop = (void (*)(ALuint))qLibrary.resolve("alSourceStop");

            funcalSource3f  = (void (*)(ALuint, ALenum, ALfloat, ALfloat, ALfloat))qLibrary.resolve("alSource3f");
            funcalSource3i  = (void (*)(ALuint, ALenum, ALint, ALint, ALint))qLibrary.resolve("alSource3i");
            funcalSourcei  = (void (*)(ALuint, ALenum, ALint))qLibrary.resolve("alSourcei");
            funcalSourcef  = (void (*)(ALuint, ALenum, ALfloat))qLibrary.resolve("alSourcef");



            funcalGenSources  = (void (*)(ALsizei, ALint*)) qLibrary.resolve("alGenSources");

            funcalBufferData=(void (*)(ALuint, ALenum, ALvoid*, ALsizei, ALsizei)) qLibrary.resolve("alBufferData");

            funcalGenBuffers= (void (*)(ALsizei, ALuint*))qLibrary.resolve("alGenBuffers");

            funcalIsExtensionPresent=(bool(*)(ALCdevice*, ALchar*))qLibrary.resolve("alcIsExtensionPresent");


            //EFX

            funcalcGetIntegerv = (void (*)(ALCdevice*, ALCenum, ALCsizei, ALCint*)) qLibrary.resolve("alcGetIntegerv");
            funcalGenAuxiliaryEffectSlots = (void (*)(ALsizei, ALuint*))qLibrary.resolve("alGenAuxiliaryEffectSlots");
            funcalGenEffects = (void(*)(ALsizei, ALuint*))qLibrary.resolve("alGenEffects");
            funcalAuxiliaryEffectSloti = (void (*)(ALuint, ALenum, ALint)) qLibrary.resolve("alAuxiliaryEffectSloti");
            funcalDeleteEffects = (void (*)(ALsizei, ALuint*))qLibrary.resolve("alDeleteEffects");
            funcalDeleteAuxiliaryEffectSlots = (void (*)(ALsizei, ALuint*))qLibrary.resolve("alDeleteAuxiliaryEffectSlots");
            funcalGenFilters = (void (*)(ALsizei, ALuint*))qLibrary.resolve("alGenFilters");
            funcalIsFilter = (bool (*)(ALuint))qLibrary.resolve("alIsFilter");
            funcalFilteri = (void (*)(ALuint, ALenum, ALint ))qLibrary.resolve("alFilteri");
            funcalFilterf = (void (*)(ALuint, ALenum, ALfloat ))qLibrary.resolve("alFilterf");
            funcalDeleteFilters= (void (*)(ALsizei, ALuint*))qLibrary.resolve("alDeleteFilters");
            funcalEffecti = (void (*)(ALuint, ALenum, ALuint))qLibrary.resolve("alEffecti");
            funcalEffectf = (void (*)(ALuint, ALenum, ALfloat))qLibrary.resolve("alEffectf");
            funcalEffectfv = (void (*)(ALuint, ALenum, const ALfloat*))qLibrary.resolve("alEffectfv");
            funcalIsEffect = (bool (*)(ALuint))qLibrary.resolve("alIsEffect");
            funcalGetEnumValue =(ALenum (*)(ALchar*))qLibrary.resolve("alGetEnumValue");
        }
        catch(...)
        {
            printf("One or more OpenAL functions can't be loaded");
            return false;
        }
    }
    else
    {
        qDebug() << "OpenAL Library not loaded: " << qLibrary.errorString();
        return false;
    }
    return(true);
#endif
}
