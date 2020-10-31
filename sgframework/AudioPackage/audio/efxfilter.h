#ifndef EFXFILTER_H
#define EFXFILTER_H

#include <al.h>


class EFXFilter
{
public:
  EFXFilter() {}
  ~EFXFilter() {}
  static int createFilter(ALuint *filter, int filterType, float gain, float gainLF, float gainHF);
  static int removeDirectFilter(ALuint source, ALuint *filter);
  static int removeAuxiliarySendFilter(ALuint source, ALuint effectSlot, ALuint *filter);
};

#endif // EFXFILTER_H
