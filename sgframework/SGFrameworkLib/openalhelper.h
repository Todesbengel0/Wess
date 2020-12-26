#ifndef OPENALHELPER_H
#define OPENALHELPER_H
#include <QDebug>
//#if defined(Q_OS_MAC)
//#include <OpenAL/al.h>
//#include <OpenAL/alc.h>
//#else
#include <al.h>
#include <alc.h>
//#endif

#define AL_ALEXT_PROTOTYPES 1
#include <efx.h>

//!
//! \brief OpenALHelper läd DLL und stellt OpenAL Funktionen bereit
//!
class OpenALHelper
{
public:
    //!
    //! \brief loadDLLAndFunctions lädt OpenAL32.dll und extrahiert Methoden aus der DLL
    //! \return Erfolg oder Misserfolg
    //!
    static bool loadDLLAndFunctions();

#if defined(Q_OS_MACXXX)
    //Z.T. leichte Abweichungen in den Prototypen, daher eigene Deklaration für OSX
    //OpenAL functions
    static ALCdevice* (*funcalcOpenDevice)(const ALchar*);
    static ALCcontext* (*funcalcCreateContext)(ALCdevice*, const ALCint*);
    static ALCboolean (*funcalalcMakeContextCurrent)(ALCcontext*);
    static ALenum(*funcalGetError)();
    static ALCboolean (*funcalcCloseDevice)(ALCdevice*);
    static void (*funcalSourcePlay)(ALuint);
    static void (*funcalSourcePause)(ALuint);
    static void (*funcalSourceStop)(ALuint);
    static void (*funcalListener3f)(ALenum, ALfloat, ALfloat, ALfloat);
    static void (*funcalSource3f)(ALuint, ALenum, ALfloat, ALfloat, ALfloat);
    static  void (*funcalSourcei)(ALuint, ALenum, ALint);
    static  void (*funcalSourcef)(ALuint, ALenum, ALfloat);
    static  void (*funcalGenSources)(ALsizei, ALuint*);
    static  void (*funcalcDestroyContext)(ALCcontext*);
    static  void (*funcalBufferData) (ALuint, ALenum, const ALvoid*, ALsizei, ALsizei);
    static void(*funcalGenBuffers)(ALsizei, ALuint*);
    static void (*funcalListenerfv)(ALenum, const ALfloat*);
    static ALboolean (*funcalIsExtensionPresent)(ALCdevice*, const ALchar*);
    static void (*funcalSource3i)(ALuint, ALenum, ALint, ALint, ALint);

    //EFX functions

    static void (*funcalcGetIntegerv)(ALCdevice*, ALCenum, ALCsizei, ALCint*);
    static ALenum (*funcalGetEnumValue)(const ALchar*);

    //On OSX unsupported functions defined empty
    static void (funcalAuxiliaryEffectSloti)(ALuint, ALenum, ALint) {}
    static void (funcalDeleteAuxiliaryEffectSlots)(ALsizei, const ALuint*) {}
    static void (funcalDeleteEffects)(ALsizei, const ALuint*) {}
    static void (funcalDeleteFilters)(ALsizei, const ALuint*) {}
    static void (funcalEffecti)(ALuint, ALenum, ALint){}
    static void (funcalEffectf)(ALuint, ALenum, ALfloat){}
    static void (funcalEffectfv)(ALuint, ALenum, const ALfloat*){}
    static void (funcalFilteri)(ALuint, ALenum, ALint ){}
    static void (funcalFilterf)(ALuint, ALenum, ALfloat ){}
    static void (funcalGenAuxiliaryEffectSlots)(ALsizei, ALuint*){}
    static void(funcalGenEffects)(ALsizei, ALuint*){}
    static void (funcalGenFilters)(ALsizei, ALuint*){}
    static ALboolean (funcalIsEffect)(ALuint){}
    static ALboolean (funcalIsFilter)(ALuint){}

#else
    //OpenAL functions
    static ALCdevice* (*funcalcOpenDevice)(ALchar*);
    static ALCcontext* (*funcalcCreateContext)(ALCdevice*, const ALCint*);
    static ALCcontext* (*funcalalcMakeContextCurrent)(ALCcontext*);
    static ALenum(*funcalGetError)();
    static void (*funcalcCloseDevice)(ALCdevice*);
    static void (*funcalSourcePlay)(ALuint);
    static void (*funcalSourcePause)(ALuint);
    static void (*funcalSourceStop)(ALuint);
    static void (*funcalListener3f)(ALenum, ALfloat, ALfloat, ALfloat);
    static void (*funcalSource3f)(ALuint, ALenum, ALfloat, ALfloat, ALfloat);
    static  void (*funcalSourcei)(ALuint, ALenum, ALint);
    static  void (*funcalSourcef)(ALuint, ALenum, ALfloat);
    static  void (*funcalGenSources)(ALsizei, ALint*);
    static  ALCcontext* (*funcalcDestroyContext)(ALCcontext*);
    static  void (*funcalBufferData) (ALuint, ALenum, ALvoid*, ALsizei, ALsizei);
    static void(*funcalGenBuffers)(ALsizei, ALuint*);
    static void (*funcalListenerfv)(ALenum, ALfloat*);
    static bool (*funcalIsExtensionPresent)(ALCdevice*, ALchar*);

    //EFX functions
    static void (*funcalcGetIntegerv)(ALCdevice*, ALCenum, ALCsizei, ALCint*);
    static void (*funcalGenAuxiliaryEffectSlots)(ALsizei, ALuint*);
    static void(*funcalGenEffects)(ALsizei, ALuint*);
    static void (*funcalAuxiliaryEffectSloti)(ALuint, ALenum, ALint);
    static void (*funcalDeleteEffects)(ALsizei, ALuint*);
    static void (*funcalDeleteAuxiliaryEffectSlots)(ALsizei, ALuint*);
    static void (*funcalGenFilters)(ALsizei, ALuint*);
    static bool (*funcalIsFilter)(ALuint);
    static void (*funcalFilteri)(ALuint, ALenum, ALint );
    static void (*funcalFilterf)(ALuint, ALenum, ALfloat );
    static void (*funcalDeleteFilters)(ALsizei, ALuint*);
    static void (*funcalEffecti)(ALuint, ALenum, ALuint);
    static void (*funcalEffectf)(ALuint, ALenum, ALfloat);
    static void (*funcalEffectfv)(ALuint, ALenum, const ALfloat*);
    static bool (*funcalIsEffect)(ALuint);
    static void (*funcalSource3i)(ALuint, ALenum, ALint, ALint, ALint);
    static ALenum (*funcalGetEnumValue)(ALchar*);
#endif

};

#endif // OPENALHELPER_H
