#ifndef EFXEFFECT_H
#define EFXEFFECT_H
#include "openalhelper.h"




class EFXEffect
{
public:  
  //! \brief Callback for configuring the effect to be set. In this callback we can set all Parameters with it's values and for the reverb-effect we can use the presets.
  //!
  //! Sample code for this call function might be:
  //! \code{.cpp}
  //! void
  //! cbConfigureEffect(ALuint *effect)
  //! {
  //!   // If effect is of type AL_EFFECT_CHORUS
  //!   alEffectf(*effect, AL_CHORUS_WAVEFORM, AL_CHORUS_WAVEFORM_TRIANGLE);
  //! }
  //! \endcode
  //! \param effect Reference to an effect created by alGenEffects().
  typedef void (*configureEffectCallback)(ALuint *effect);

  EFXEffect(ALuint source, ALCdevice *device);
  ~EFXEffect();
  //int setEffect(ALint effectType, configureEffectCallback configureEffect, ALint filterType = AL_FILTER_NULL, float gain = 0.5, float gainLF = 0.5, float gainHF = 0.5);
  int removeEffect();

  bool beforeSetParameters(ALint effectType, ALuint* outEffect);
  bool afterSetParameters(ALint filterType, float gain, float gainLF, float gainHF);

protected:
  ALuint source;
  ALCdevice *device;
  ALuint effectSlot[1];
  ALuint effect[1];
  ALuint filter[1];
  bool effectSet;
};

#endif // EFXEFFECT_H
