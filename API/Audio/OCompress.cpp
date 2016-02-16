#include "OCompress.hh"

OCompress::OCompress(const int samplingRate, const int frameSize)
{
  this->_samplingRate = samplingRate;
  this->_frameSize = frameSize;
  this->_maxDataBytes = (this->_samplingRate * 1024);
  this->_compressedSize = 0;
  this->_encoder = opus_encoder_create(this->_samplingRate, NB_CHANNEL, OPUS_APPLICATION_VOIP, &this->_eError);
  this->_decoder = opus_decoder_create(this->_samplingRate, NB_CHANNEL, &this->_dError);
  if (this->_eError != OPUS_OK || this->_dError != OPUS_OK)
    {
      std::cerr << "encoder = " << this->_eError << std::endl;
      std::cerr << "decoder = " << this->_dError << std::endl;
    }
  opus_encoder_ctl(this->_encoder, OPUS_GET_BANDWIDTH(&this->_maxDataBytes));
  this->_compressedData = new unsigned char[this->_maxDataBytes];
}

OCompress::~OCompress()
{
  opus_encoder_destroy(this->_encoder);
  opus_decoder_destroy(this->_decoder);
  delete this->_compressedData;
}

void OCompress::wavToCompressed(const float *pcm)
{
  if (!(this->_compressedSize = opus_encode_float(this->_encoder, pcm, this->_frameSize, this->_compressedData, this->_maxDataBytes)))
    std::cerr << opus_strerror(_compressedSize) << std::endl;
}

void OCompress::compressedToWav(const unsigned char *data, int len, float *decodedData)
{
  if (!(this->_err = opus_decode_float(_decoder, data, len, decodedData, _frameSize, DECODE_FEC)))
    std::cerr << opus_strerror(this->_err) << std::endl;
}
