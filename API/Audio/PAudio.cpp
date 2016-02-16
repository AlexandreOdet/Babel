#include "PAudio.hh"

/**
 * \file PAudio.cpp
 * \brief File creating a PAudio instance with its methods
 * \author Odet Alexandre
 * \date 8 November 2015
 * Implementation of PAudio class, containing the stream handling (open, close, start, stop).
 */

static int callbackRecordFunction(const void *inputBuffer,
			     void *outputBuffer, unsigned long framesPerBuffer,
			     const PaStreamCallbackTimeInfo* timeInfo,
			     PaStreamCallbackFlags statusFlags,
			     void *userData);

static int callbackReplayFunction(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData);
/**
	* \fn PAudio::PAudio()
	* \brief PAudio class constructor, initialize the input device and library
*/

PAudio::PAudio()
{
  this->_numberOfDevices = 0;
  this->_err = Pa_Initialize();
  if (this->_err != paNoError)
    std::cerr << "PortAudio Error : " <<  Pa_GetErrorText(this->_err) << std::endl;
	this->_inputParameters.device = Pa_GetDefaultInputDevice();
  if (this->_inputParameters.device == paNoDevice)
		std::cerr << "No input device" << std::endl;
	else
	{
		this->_inputParameters.channelCount = 2;
		this->_inputParameters.sampleFormat = PA_SAMPLE_TYPE;
		this->_inputParameters.suggestedLatency = Pa_GetDeviceInfo(this->_inputParameters.device)->defaultLowInputLatency;
		this->_inputParameters.hostApiSpecificStreamInfo = NULL;
	}
	this->_data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
	this->_data.frameIndex = 0;
	numSamples = totalFrames * NUM_CHANNELS;
	numBytes = numSamples * sizeof(SAMPLE);
	this->_data.recordedSamples = new SAMPLE[numBytes];
	for (int i = 0; i < numBytes; i++)
		this->_data.recordedSamples[i] = 0;
}

/**
	* \fn ~PAudio::PAudio()
	* \brief PAudio class destructor, delete the sample recorded, and terminate the use of Port Audio.
*/

PAudio::~PAudio()
{
	delete this->_data.recordedSamples;
  this->_err = Pa_Terminate();
  if (this->_err != paNoError)
    {
      std::cerr << "PortAudio Error : " <<  Pa_GetErrorText(this->_err) << std::endl;
    }
}

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvelle allouée d'un objet de type Str_t ou NULL.
 */

bool  PAudio::openStream(const int functionToCall)
{
	this->_err = Pa_OpenStream(
																&this->_stream,
                                (functionToCall) ? &this->_inputParameters : NULL,
                                (functionToCall) ? NULL : &this->_outputParameters,
                                SAMPLE_RATE,
																FRAMES_PER_BUFFER,
																paClipOff,
                                (functionToCall) ? callbackRecordFunction : callbackReplayFunction,
                                &this->_data);
  if (this->_err != paNoError)
    {
      std::cerr << "openStream error : " << Pa_GetErrorText(this->_err) << std::endl;
      return false;
    }
  return true;
}

bool PAudio::startStream()
{
 	if ((this->_err = Pa_StartStream(this->_stream)) != paNoError)
		return false;
	while ((this->_err = Pa_IsStreamActive(this->_stream)) == 1)
		Pa_Sleep(1000);
  return true;
}

bool PAudio::stopStream()
{
  this->_err = Pa_StopStream(this->_stream);
  if (this->_err != paNoError)
  {
    std::cerr << "StopStream error : " << Pa_GetErrorText(this->_err) << std::endl;
    return false;
  }
  return true;
}

bool PAudio::closeStream()
{
  this->_err = Pa_CloseStream(this->_stream);
  if (this->_err != paNoError)
    {
      std::cerr << "closeStream error : " << Pa_GetErrorText(this->_err) << std::endl;
      return false;
    }
  return true;
}

size_t PAudio::getAllDevices()
{
  int numDevices = Pa_GetDeviceCount();
  if (numDevices < 0)
    {
      std::cerr << "Error while counting devices, Pa_CountDevices returned : " << numDevices << std::endl;
      //throw exception
    }
  else
    this->_numberOfDevices = numDevices;
  return this->_numberOfDevices;
}

static int callbackRecordFunction(const void *inputBuffer,
			     void *outputBuffer, unsigned long framesPerBuffer,
			     const PaStreamCallbackTimeInfo* timeInfo,
			     PaStreamCallbackFlags statusFlags,
			     void *userData)
{
	paTestData *data = (paTestData*)userData;
	const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
	SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	long framesToCalc;
	long i;
	int finished;
	unsigned long framesLeft = (data->maxFrameIndex - data->frameIndex);

	if (framesLeft < framesPerBuffer)
	{
		framesToCalc = framesLeft;
		finished = paComplete;
	}
	else
	{
		framesToCalc = framesPerBuffer;
		finished = paContinue;
	}

	if (inputBuffer == NULL)
	{
		for(i = 0; i < framesToCalc; i++)
			{
				*wptr++ = SAMPLE_SILENCE;  /* left */
				if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
			}
	}
	else
	{
			for(i = 0; i < framesToCalc; i++)
			{
				*wptr++ = *rptr++;  /* left */
				if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
			}
	}
	data->frameIndex += framesToCalc;
	return finished;
	(void)timeInfo;
	(void)statusFlags;
	(void)inputBuffer;
	(void)outputBuffer;
}

static int callbackReplayFunction(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
	paTestData *data = (paTestData*)userData;
	SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	SAMPLE *wptr = (SAMPLE*)outputBuffer;
	unsigned int i;
	unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;
int finished;

	if (framesLeft < framesPerBuffer)
	{
		finished = paComplete;
		for(i = 0; i < framesLeft; i++)
		{
			*wptr++ = *rptr++;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
		}
		for(; i < framesPerBuffer; i++)
		{
			*wptr++ = 0;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = 0;  /* right */
		}
		data->frameIndex += framesLeft;
	}
	else
	{
		finished = paContinue;
			for(i = 0; i < framesPerBuffer; i++)
			{
				*wptr++ = *rptr++;  /* left */
				if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
			}
			data->frameIndex += framesPerBuffer;
	}
	return finished;
	(void)timeInfo;
	(void)statusFlags;
	(void)inputBuffer;
}

double PAudio::getStreamTime() const
{
  double time = Pa_GetStreamTime(this->_stream);
  return time;
}

void  PAudio::replayStream()
{
	this->_data.frameIndex = 0;

	this->_outputParameters.device = Pa_GetDefaultOutputDevice();
	if (this->_outputParameters.device == paNoDevice)
	{
		std::cerr << "No output device" << std::endl;
	}
	this->_outputParameters.channelCount = 2;
	this->_outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
	this->_outputParameters.suggestedLatency = Pa_GetDeviceInfo(this->_outputParameters.device)->defaultLowOutputLatency;
	this->_outputParameters.hostApiSpecificStreamInfo = NULL;
}
