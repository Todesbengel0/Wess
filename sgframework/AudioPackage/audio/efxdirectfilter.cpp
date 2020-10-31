#include "efxdirectfilter.h"
#include "openalhelper.h"
#include "efxfilter.h"


//! \brief Creates a EFX direct-filter-object.
//!
//! \param source The source-ID of a OpenAL-sound-source by alGenSources().
EFXDirectFilter::EFXDirectFilter(ALuint source)
{
  this->filterSet = false;
  this->source = source;
}


//! \brief Destroys a EFX direct-filter-object.
EFXDirectFilter::~EFXDirectFilter()
{
  removeDirectFilter();
}


//! \brief Sets a low pass filter to this source.
//!
//! \param gain Gain-value.
//! \param gainHF Gain-value for high frequencies.
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXDirectFilter::setDirectLowPassFilter(float gain, float gainHF)
{
  return setDirectPassFilter(AL_FILTER_LOWPASS, gain, 0, gainHF);
}


//! \brief Sets a high pass filter to this source.
//!
//! \param gain Gain-value.
//! \param gainLF Gain-value for low frequencies.
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXDirectFilter::setDirectHighPassFilter(float gain, float gainLF)
{
  return setDirectPassFilter(AL_FILTER_HIGHPASS, gain, gainLF, 0);
}


//! \brief Sets a band pass filter to this source.
//!
//! \param gain Gain-value.
//! \param gainLF Gain-value for low frequencies.
//! \param gainHF Gain-value for high frequencies.
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXDirectFilter::setDirectBandPassFilter(float gain, float gainLF, float gainHF)
{
  return setDirectPassFilter(AL_FILTER_BANDPASS, gain, gainLF, gainHF);
}


//! \brief Removes the set filter from this source
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXDirectFilter::removeDirectFilter()
{
  if(!this->filterSet)
    return -1;

  if(EFXFilter::removeDirectFilter(this->source, &this->filter[0]) != 0)
    return -2;

  this->filterSet = false;

  return 0;
}


//! \brief Generic function for creating all types of filter.
//!
//! \param filterType Filter-type - see AL/efx.h. There are only a few filter types: AL_FILTER_NULL, AL_FILTER_LOWPASS, AL_FILTER_HIGHPASS, AL_FILTER_BANDPASS.
//! \param gain Gain-value.
//! \param gainLF Gain-value for low frequencies.
//! \param gainHF Gain-value for high frequencies.
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXDirectFilter::setDirectPassFilter(int filterType, float gain, float gainLF, float gainHF)
{
  if(EFXFilter::createFilter(&this->filter[0], filterType, gain, gainLF, gainHF) != 0)
    return -1;

  // Activate the filter (needs active ALC context)
  OpenALHelper::funcalGetError();

  OpenALHelper::funcalSourcei(this->source, AL_DIRECT_FILTER, this->filter[0]);

  if ( OpenALHelper::funcalGetError() != AL_NO_ERROR)
    return -2;

  this->filterSet = true;

  return 0;
}
