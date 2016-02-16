#ifndef __OCOMPRESS__
#define __OCOMPRESS__

#define NB_CHANNEL (1)
#define DECODE_FEC (0)

#include "ICompress.hh"
#ifndef _WIN32
#include <opus/opus.h>
#include <opus/opus_defines.h>
#include <opus/opus_types.h>
#endif
#ifdef _WIN32
#include <opus.h>
#include <opus_defines.h>
#include <opus_types.h>
#endif
#include <iostream>


class OCompress : public ICompress
{
public:
  OCompress(const int, const int);
  virtual ~OCompress();
  virtual void wavToCompressed(const float *);
  virtual void compressedToWav(const unsigned char *, int, float *);
private:
  OpusEncoder *_encoder;
  OpusDecoder *_decoder;
  int _samplingRate;
  int _maxDataBytes;
  int	_frameSize;
  int _compressedSize;
  unsigned char* _compressedData;
  int _eError;
  int _dError;
  int _err;
};

#endif /*__OCOMPRESS__*/
