#ifndef __PortAudio_HH__
#define __PortAudio_HH__

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (5)
#define NUM_CHANNELS    (2)
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0) /**/
/** Set to 1 if you want to capture the recording to a file. */

/* Select sample format. */

#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif

#include "IAudio.hh"
#include "portaudio.h"
#include <vector>

typedef struct  paTestData
{
  int          frameIndex;  /* Index into sample array. */
  int          maxFrameIndex;
  SAMPLE      *recordedSamples;
}                paTestData;

class PAudio : public IAudio
{
  public:
    PAudio();
    virtual ~PAudio();
    virtual bool openStream(const int);
    virtual bool closeStream();
    virtual bool startStream();
    virtual bool stopStream();
    virtual size_t getAllDevices();
    virtual double getStreamTime() const;
    virtual void replayStream();
  private:
    size_t _numberOfDevices;
    PaStream *_stream;
    PaStreamParameters _inputParameters, _outputParameters;
    std::vector<SAMPLE> _sampleRecorded;
    int totalFrames, numSamples, numBytes;
    PaError _err;
    paTestData _data;
};



#endif /*__PortAudio_HH__*/
