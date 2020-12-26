#ifndef EFXDIRECTFILTER_H
#define EFXDIRECTFILTER_H
#include "openalhelper.h"
#include <al.h>


class EFXDirectFilter
{
public:
  EFXDirectFilter(ALuint source);
  ~EFXDirectFilter();
  int setDirectLowPassFilter(float gain, float gainHF);
  int setDirectHighPassFilter(float gain, float gainLF);
  int setDirectBandPassFilter(float gain, float gainLF, float gainHF);
  int removeDirectFilter();

private:
  ALuint source;
  ALuint filter[1] = { 0 };
  bool filterSet;

  int setDirectPassFilter(int filterType, float gain, float gainLF, float gainHF);
};

#endif // EFXDIRECTFILTER_H
