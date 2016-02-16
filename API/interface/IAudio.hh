#ifndef __IAUDIO__
#define __IAUDIO__

#include <iostream>

class IAudio
{
public:
  virtual ~IAudio(){}
  virtual bool openStream(const int) = 0;
  virtual bool closeStream() = 0;
  virtual bool startStream() = 0;
  virtual bool stopStream() = 0;
  virtual std::size_t getAllDevices() = 0;
  virtual double getStreamTime() const = 0;
  virtual void replayStream() = 0;
};

#endif /*__IAUDIO__*/
