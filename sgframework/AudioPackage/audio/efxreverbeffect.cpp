#include "efxreverbeffect.h"
#include "efxdirectfilter.h"
#include "openalhelper.h"



//! \brief Loads the given reverb properties into a new OpenAL effect object, and returns the new effect ID.
//!
//! \param reverb Reference to an EFXEAXREVERBPROPERTIES-structure - see AL/efx-presets.h) we will use to configure the given effect.
//! \param effect Reference to the effect-object (created by alGenEffect()) where to apply the settings.
//! \param[out] eaxReverbAvailable Infdicates, if the EAX Reverb Properties (which are a superset of the EFX ones) are available.
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXReverbEffect::loadReverbEffect(const EFXEAXREVERBPROPERTIES reverb, ALuint *effect, bool *eaxReverbAvailable)
{
  // Create the effect object and check if we can do EAX reverb
  if(OpenALHelper::funcalGetEnumValue((ALchar *)"AL_EFFECT_EAXREVERB") != 0)
  {
    if(eaxReverbAvailable)
      *eaxReverbAvailable = true;

    // EAX Reverb is available. Set the EAX effect type then load the reverb properties
    OpenALHelper::funcalGetError();
    OpenALHelper::funcalEffecti(*effect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);

    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_DENSITY, reverb.flDensity);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_DIFFUSION, reverb.flDiffusion);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_GAIN, reverb.flGain);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_GAINHF, reverb.flGainHF);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_GAINLF, reverb.flGainLF);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_DECAY_TIME, reverb.flDecayTime);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_DECAY_HFRATIO, reverb.flDecayHFRatio);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_DECAY_LFRATIO, reverb.flDecayLFRatio);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_REFLECTIONS_GAIN, reverb.flReflectionsGain);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_REFLECTIONS_DELAY, reverb.flReflectionsDelay);
    OpenALHelper::funcalEffectfv(*effect, AL_EAXREVERB_REFLECTIONS_PAN, reverb.flReflectionsPan);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_LATE_REVERB_GAIN, reverb.flLateReverbGain);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_LATE_REVERB_DELAY, reverb.flLateReverbDelay);
    OpenALHelper::funcalEffectfv(*effect, AL_EAXREVERB_LATE_REVERB_PAN, reverb.flLateReverbPan);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_ECHO_TIME, reverb.flEchoTime);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_ECHO_DEPTH, reverb.flEchoDepth);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_MODULATION_TIME, reverb.flModulationTime);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_MODULATION_DEPTH, reverb.flModulationDepth);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, reverb.flAirAbsorptionGainHF);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_HFREFERENCE, reverb.flHFReference);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_LFREFERENCE, reverb.flLFReference);
    OpenALHelper::funcalEffectf(*effect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, reverb.flRoomRolloffFactor);
    OpenALHelper::funcalEffecti(*effect, AL_EAXREVERB_DECAY_HFLIMIT, reverb.iDecayHFLimit);

    if(OpenALHelper::funcalGetError() != AL_NO_ERROR)
      return -1;
  }
  else
  {
    if(eaxReverbAvailable)
      *eaxReverbAvailable = false;

    // No EAX Reverb. Set the standard reverb effect type then load the available reverb properties
    OpenALHelper::funcalGetError();
    OpenALHelper::funcalEffecti(*effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

    OpenALHelper::funcalEffectf(*effect, AL_REVERB_DENSITY, reverb.flDensity);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_DIFFUSION, reverb.flDiffusion);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_GAIN, reverb.flGain);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_GAINHF, reverb.flGainHF);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_DECAY_TIME, reverb.flDecayTime);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_DECAY_HFRATIO, reverb.flDecayHFRatio);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_REFLECTIONS_GAIN, reverb.flReflectionsGain);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_REFLECTIONS_DELAY, reverb.flReflectionsDelay);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_LATE_REVERB_GAIN, reverb.flLateReverbGain);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_LATE_REVERB_DELAY, reverb.flLateReverbDelay);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_AIR_ABSORPTION_GAINHF, reverb.flAirAbsorptionGainHF);
    OpenALHelper::funcalEffectf(*effect, AL_REVERB_ROOM_ROLLOFF_FACTOR, reverb.flRoomRolloffFactor);
    OpenALHelper::funcalEffecti(*effect, AL_REVERB_DECAY_HFLIMIT, reverb.iDecayHFLimit);

    if(OpenALHelper::funcalGetError() != AL_NO_ERROR)
      return -2;
  }

  return 0;
}


//! \brief Creates a reverb effect combined with a low pass filter.
//!
//! \param configureReverbEffect Callback for configuring the reverb effect to set.
//! \param gain Gain-value.
//! \param gainHF Gain-value for high frequencies.
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXReverbEffect::setReverbEffect(const EFXEAXREVERBPROPERTIES reverb, float gain, float gainHF)
{
    ALuint* effect=nullptr;
    bool result = EFXEffect::beforeSetParameters(AL_EFFECT_REVERB, effect);

    if(result && effect)
    {
        this->loadReverbEffect(reverb, effect);
    }
    EFXEffect::afterSetParameters(AL_FILTER_LOWPASS, gain, 0, gainHF);

    //return EFXEffect::setEffect(AL_EFFECT_REVERB, AL_FILTER_LOWPASS, configureReverbEffect, gain, 0, gainHF);
    effect = nullptr;
   return(0);
}


//! \brief Removes the reverb effect from the given source.
//!
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXReverbEffect::removeReverbEffect()
{
  return EFXEffect::removeEffect();
}
