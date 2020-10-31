#include "efxeffect.h"
#include "efxfilter.h"
#include "openalhelper.h"
#include <stddef.h>


//! \brief Creates a EFX effect-object.
//!
//! \param source The source-ID of a OpenAL-sound-source by alGenSources().
//! \param device The audio library context we will apply the effect to.
EFXEffect::EFXEffect(ALuint source, ALCdevice *device)
{
  this->effectSet = false;
  this->source = source;
  this->device = device;
  this->effectSlot[0] = 0;
  this->effect[0] = 0;
  this->filter[0] = 0;
}


//! \brief Destroys a EFX effect-object.
EFXEffect::~EFXEffect()
{
  removeEffect();
}


//! \brief Generic function for creating any effect combined with any filter.
//!
//! \param effectType Effect-type - see AL/efx.h. There are multiple effect types, e.g. AL_EFFECT_NULL, AL_EFFECT_REVERB, AL_EFFECT_CHORUS.
//! \param configureEffect Callback for configuring the effect to set.
//! \param filterType Filter-type - see AL/efx.h. There are only a few filter types: AL_FILTER_NULL, AL_FILTER_LOWPASS, AL_FILTER_HIGHPASS, AL_FILTER_BANDPASS.
//! \param gain Gain-value.
//! \param gainLF Gain-value for low frequencies.
//! \param gainHF Gain-value for high frequencies.
//! \return Error-code. 0 = No error, <0 = error occured.
//int EFXEffect::setEffect(ALint effectType, ALint filterType, float gain, float gainLF, float gainHF)
//{
//  ALCint numSends = 0;

//  // Retrieve the actual number of Aux Sends available on each Source
//  alcGetIntegerv(this->device, ALC_MAX_AUXILIARY_SENDS, 1, &numSends);

//  if(numSends < 1) // We need (at least) 1 Auxiliary Send
//    return -1;

//  // Create Auxiliary Effect Slot
//  alGetError();
//  alGenAuxiliaryEffectSlots(1, &this->effectSlot[0]);
//  if (alGetError() != AL_NO_ERROR)
//    return -2;

//  // Create Effect
//  alGenEffects(1, &this->effect[0]);
//  if (alGetError() != AL_NO_ERROR)
//    return -3;

//  // Set Effect Type and change attributes
//  alGetError();
//  if (alIsEffect(this->effect[0]))
//  {
//    alEffecti(this->effect[0], AL_EFFECT_TYPE, effectType);
//    if (alGetError() != AL_NO_ERROR)
//      return -4;
//    else
//      if(configureEffect)
//        configureEffect(&this->effect[0]);
//  }

//  // Attach Effect to Auxiliary Effect Slot
//  alAuxiliaryEffectSloti(this->effectSlot[0], AL_EFFECTSLOT_EFFECT, this->effect[0]);
//  if (alGetError() != AL_NO_ERROR)
//    return -5;

//  if(EFXFilter::createFilter(&this->filter[0], filterType, gain, gainLF, gainHF) != 0)
//    return -6;

//  // Filter the Source send 0 from 'source' to Auxiliary Effect Slot using Filter
//  alSource3i(this->source, AL_AUXILIARY_SEND_FILTER, effectSlot[0], 0, this->filter[0]);

//  this->effectSet = true;

//  return 0;
//}



bool EFXEffect::beforeSetParameters(ALint effectType, ALuint* outEffect)
{
    ALCint numSends = 0;

    // Retrieve the actual number of Aux Sends available on each Source
    OpenALHelper::funcalcGetIntegerv(this->device, ALC_MAX_AUXILIARY_SENDS, 1, &numSends);

    if(numSends < 1) // We need (at least) 1 Auxiliary Send
      return false;

    // Create Auxiliary Effect Slot
    OpenALHelper::funcalGetError();
     OpenALHelper::funcalGenAuxiliaryEffectSlots(1, &this->effectSlot[0]);
    if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
      return false;

    // Create Effect
     OpenALHelper::funcalGenEffects(1, &this->effect[0]);
    if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
      return false;

    // Set Effect Type and change attributes
    OpenALHelper::funcalGetError();
    if ( OpenALHelper::funcalIsEffect(this->effect[0]))
    {
       OpenALHelper::funcalEffecti(this->effect[0], AL_EFFECT_TYPE, effectType);
      if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
        return false;

      outEffect= &this->effect[0];
      return true;
    }
    return false;
}

bool EFXEffect::afterSetParameters( ALint filterType, float gain, float gainLF, float gainHF)
{
    // Attach Effect to Auxiliary Effect Slot
     OpenALHelper::funcalAuxiliaryEffectSloti(this->effectSlot[0], AL_EFFECTSLOT_EFFECT, this->effect[0]);
    if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
      return false;

    if(EFXFilter::createFilter(&this->filter[0], filterType, gain, gainLF, gainHF) != 0)
      return false;

    // Filter the Source send 0 from 'source' to Auxiliary Effect Slot using Filter
     OpenALHelper::funcalSource3i(this->source, AL_AUXILIARY_SEND_FILTER, effectSlot[0], 0, this->filter[0]);

    this->effectSet = true;

    return true;
}


//! \brief Removes the effect from the given source.
//!
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXEffect::removeEffect()
{
  if(!this->effectSet)
    return -1;

  OpenALHelper::funcalGetError();
  //Disable Auxiliary Send Filter
   OpenALHelper::funcalSource3i(source, AL_AUXILIARY_SEND_FILTER, this->effectSlot[0], 0, AL_FILTER_NULL);
  if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
    return -2;

  // Delete filter
  if(EFXFilter::removeAuxiliarySendFilter(this->source, this->effectSlot[0], &this->filter[0]) != 0)
    return -3;

  // Delete Effect
  OpenALHelper::funcalDeleteEffects(1, &this->effect[0]);
  if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
    return -4;

  // Delete AuxiliaryEffectSlot
  OpenALHelper::funcalDeleteAuxiliaryEffectSlots(1, &this->effectSlot[0]);
  if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
    return -5;

  this->effectSet = false;

  return 0;
}
