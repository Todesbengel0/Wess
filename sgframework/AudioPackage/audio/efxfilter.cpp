#include "efxfilter.h"
#include "openalhelper.h"


//! \brief Generic function for creating all types of filter.
//!
//! \param filter Reference to the filter-object (created by alGenFilters()) where to store the newly created filter object.
//! \param filterType Filter-type - see AL/efx.h. There are only a few filter types: AL_FILTER_NULL, AL_FILTER_LOWPASS, AL_FILTER_HIGHPASS, AL_FILTER_BANDPASS.
//! \param gain Gain-value.
//! \param gainLF Gain-value for low frequencies.
//! \param gainHF Gain-value for high frequencies.
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXFilter::createFilter(ALuint *filter, int filterType, float gain, float gainLF, float gainHF)
{
  // Create a Filter
  OpenALHelper::funcalGetError();
  OpenALHelper::funcalGenFilters(1, filter);

  if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
    return -1;

  if (OpenALHelper::funcalIsFilter(*filter))
  {
    // Set Filter type and parameters
    OpenALHelper::funcalFilteri(*filter, AL_FILTER_TYPE, filterType);

    if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
      return -2;
    else
    {
      if(filterType == AL_FILTER_LOWPASS) {
        OpenALHelper::funcalFilterf(*filter, AL_LOWPASS_GAIN, gain);
        OpenALHelper::funcalFilterf(*filter, AL_LOWPASS_GAINHF, gainHF);
      } else if(filterType == AL_FILTER_HIGHPASS) {
        OpenALHelper::funcalFilterf(*filter, AL_HIGHPASS_GAIN, gain);
        OpenALHelper::funcalFilterf(*filter, AL_HIGHPASS_GAINLF, gainLF);
      } else if(filterType == AL_FILTER_BANDPASS) {
        OpenALHelper::funcalFilterf(*filter, AL_BANDPASS_GAIN, gain);
        OpenALHelper::funcalFilterf(*filter, AL_BANDPASS_GAINLF, gainLF);
        OpenALHelper::funcalFilterf(*filter, AL_BANDPASS_GAINHF, gainHF);
      }

      if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
        return -3;
    }
  }

  return 0;
}


//! \brief Generic function for creating all types of filter.
//!
//! \param source The source-ID of a OpenAL-sound-source by alGenSources().
//! \param filter Reference to the filter-object where to remove the newly created filter object.
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXFilter::removeDirectFilter(ALuint source, ALuint *filter)
{
  if(!filter)
    return -1;

  // Destroy filter
  OpenALHelper::funcalGetError();
  OpenALHelper::funcalSourcei(source, AL_DIRECT_FILTER, AL_FILTER_NULL);
  if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
    return -2;

  // Delete filter
  OpenALHelper::funcalDeleteFilters(1, filter);

  if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
    return -3;

  return 0;
}


//! \brief Generic function for creating all types of filter.
//!
//! \param source The source-ID of a OpenAL-sound-source by alGenSources().
//! \param filter Reference to the filter-object where to remove the newly created filter object.
//! \param effectSlot Reference to the effect-slot where to remove the newly created filter object.
//! \return Error-code. 0 = No error, <0 = error occured.
int EFXFilter::removeAuxiliarySendFilter(ALuint source, ALuint effectSlot, ALuint *filter)
{
  if(!filter)
    return -1;

  // Destroy filter
  OpenALHelper::funcalGetError();
  OpenALHelper::funcalSource3i(source, AL_AUXILIARY_SEND_FILTER, effectSlot, 0, AL_FILTER_NULL);
  if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
    return -2;

  // Delete filter
  OpenALHelper::funcalDeleteFilters(1, filter);

  if (OpenALHelper::funcalGetError() != AL_NO_ERROR)
    return -3;

  return 0;
}
