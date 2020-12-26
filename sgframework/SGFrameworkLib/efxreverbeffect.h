#ifndef EFXREVERBEFFECT_H
#define EFXREVERBEFFECT_H

#include <efx-presets.h>
#include "efxeffect.h"


class EFXReverbEffect : public EFXEffect
{
public:
  //! \brief Callback for configuring the reverb effect to be set. In this callback we can set all Parameters with it's values and we can use the reverb-effect-presets.
  //!
  //! Sample code for this call function might be:
  //! \code{.cpp}
  //! void
  //! cbConfigureReverbEffect(ALuint *effect)
  //! {
  //!   int manner = 1;
  //!
  //!   if(manner == 1) {
  //!     EFXEAXREVERBPROPERTIES reverb = EFX_REVERB_PRESET_BATHROOM; //EFX_REVERB_PRESET_GENERIC;
  //!     reverb.flDecayTime = 20;
  //!     EFXReverbEffect::loadReverbEffect(&reverb, effect);
  //!   } else if(manner == 2) {
  //!     alEffectf(*effect, AL_REVERB_DECAY_TIME, 20.0f);
  //!   }
  //! }
  //! \endcode
  //! \param effect Reference to an effect created by alGenEffects().
  typedef void (*configureReverbEffectCallback)(ALuint *effect);

  EFXReverbEffect(ALuint source, ALCdevice *device) : EFXEffect(source, device) { }
  ~EFXReverbEffect() { }
  static int loadReverbEffect(const EFXEAXREVERBPROPERTIES reverb, ALuint *effect, bool *eaxReverbAvailable = 0);
  int setReverbEffect(const EFXEAXREVERBPROPERTIES reverb, float gain, float gainHF);
  int removeReverbEffect();
};

#endif // EFXREVERBEFFECT_H
