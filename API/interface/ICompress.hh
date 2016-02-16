#ifndef __ICompress__
#define __ICompress__

class ICompress
{
public:
  virtual ~ICompress(){}
  virtual void wavToCompressed(const float *) = 0;
  virtual void compressedToWav(const unsigned char *, int, float *) = 0;
};

#endif /*__ICompress__*/
