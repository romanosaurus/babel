/*
** EPITECH PROJECT, 2019
** babel
** File description:
** OpusCodecAPI
*/

/* Created the 23/09/2019 at 13:38 by charlie.jeanneau@epitech.eu*/

#include "codec/OpusCodecAPI.hpp"
#include "logger/DefaultLogger.hpp"
#include <iostream>

b12software::babel::OpusCodecAPI::OpusCodecAPI()
    : _bitRate(0), _complexity(0), _nbChannels(0), _samplingRate(0), _encoderInitialized(false),
    _decoderInitialized(false), _encoder(nullptr), _decoder(nullptr), _errEncoder(0), _errDecoder(0)
{

}

b12software::babel::OpusCodecAPI::~OpusCodecAPI()
{
    if (isDecoderInitialized() == true) {
        opus_decoder_destroy(_decoder);
        _decoderInitialized = false;
        DefaultLogger::Log(LogLevelDebug, std::string("[OpusCodecAPI] decoder correctly destroy"));
    }
    else
        DefaultLogger::Log(LogLevelDebug, std::string("[OpusCodecAPI] decoder not initialize, impossible to destroy him"));
    if (isEncoderInitialized() == true) {
        opus_encoder_destroy(_encoder);
        _encoderInitialized = false;
        DefaultLogger::Log(LogLevelDebug, std::string("[OpusCodecAPI] encoder correctly destroy"));
    }
    else
        DefaultLogger::Log(LogLevelDebug, std::string("[OpusCodecAPI] encoder not initialize, impossible to destroy him"));
}

void b12software::babel::OpusCodecAPI::setBitRate(std::int32_t bitRate)
{
    if (_bitRate == bitRate)
        return;
    _bitRate = bitRate;
    DefaultLogger::Log(LogLevelDebug, std::string("[OpusCodecAPI] Number of bitRate set to ") + std::to_string(_bitRate));
}

void b12software::babel::OpusCodecAPI::setComplexity(std::int32_t complexity)
{
    if (_complexity == complexity)
        return;
    _complexity = complexity;
    DefaultLogger::Log(LogLevelDebug, std::string("[OpusCodecAPI] Number of complexity set to ") + std::to_string(_complexity));
}

void b12software::babel::OpusCodecAPI::setNbChannels(size_t nbChannel)
{
    if (_nbChannels == nbChannel)
        return;
    _nbChannels = nbChannel;
    DefaultLogger::Log(LogLevelDebug, std::string("[OpusCodecAPI] Number of nbChannel set to ") + std::to_string(_nbChannels));
}

void b12software::babel::OpusCodecAPI::setSamplingRate(std::int32_t samplingRate)
{
    if (_samplingRate == samplingRate)
        return;
    _samplingRate = samplingRate;
    DefaultLogger::Log(LogLevelDebug, std::string("[OpusCodecAPI] Number of samplingRate set to ") + std::to_string(_samplingRate));
}

std::int32_t b12software::babel::OpusCodecAPI::getBitRate() const
{
    return _bitRate;
}

std::int32_t b12software::babel::OpusCodecAPI::getComplexity() const
{
    return _complexity;
}

size_t b12software::babel::OpusCodecAPI::getNbChannels() const
{
    return _nbChannels;
}

std::int32_t b12software::babel::OpusCodecAPI::getSamplingRate() const
{
    return _samplingRate;
}

bool b12software::babel::OpusCodecAPI::isEncoderInitialized()
{
    return _encoderInitialized;
}

bool b12software::babel::OpusCodecAPI::isDecoderInitialized()
{
    return _decoderInitialized;
}

int b12software::babel::OpusCodecAPI::encode(const std::int16_t *pcm, int frameSize, unsigned char *data, size_t dataSize)
{
    if (isEncoderInitialized() == true) {
        _errEncoder = opus_encode(_encoder, pcm, frameSize, data, dataSize);
        if (_errEncoder > 0) {
            return _errEncoder;
        }
        else {
            DefaultLogger::Log(LogLevelError, std::string("[OpusCodecAPI] Fail to encode int pcm, error is -> ") + std::to_string(_errEncoder));
            return -1;
        }
    }
    else {
        DefaultLogger::Log(LogLevelWarn, std::string("[OpusCodecAPI] encoder not initialized"));
        return -1;
    }
}

int b12software::babel::OpusCodecAPI::encode(const float *pcm, int frameSize, unsigned char *data, size_t dataSize)
{
    if (isEncoderInitialized() == true) {
        _errEncoder = opus_encode_float(_encoder, pcm, frameSize, data, dataSize);
        if (_errEncoder > 0) {
            return _errEncoder;
        } else {
            DefaultLogger::Log(LogLevelError, std::string("[OpusCodecAPI] Fail to encode float pcm, error is -> ") + std::to_string(_errEncoder));
            return -1;
        }
    }
    else {
        DefaultLogger::Log(LogLevelWarn, std::string("[OpusCodecAPI] encoder not initialized"));
        return -1;
    }
}

int b12software::babel::OpusCodecAPI::decode(const unsigned char *data, size_t dataSize, std::int16_t *pcm, int frameSize)
{
    if (isDecoderInitialized() == true) {
        _errDecoder = opus_decode(_decoder, data, dataSize, pcm, frameSize, 0);
        if (_errDecoder > 0) {
            return _errDecoder;
        }
        else {
            DefaultLogger::Log(LogLevelError, std::string("[OpusCodecAPI] Fail to decode int pcm, error is -> ") + std::to_string(_errDecoder));
            return -1;
        }
    }
    else {
        DefaultLogger::Log(LogLevelWarn, std::string("[OpusCodecAPI] decoder not initialized"));
        return -1;
    }
}

int b12software::babel::OpusCodecAPI::decode(const unsigned char *data, size_t dataSize, float *pcm, int frameSize)
{
    if (isDecoderInitialized() == true) {
        _errDecoder = opus_decode_float(_decoder, data, dataSize, pcm, frameSize, 0);
        if (_errDecoder > 0) {
            return _errDecoder;
        }
        else {
            DefaultLogger::Log(LogLevelError, std::string("[OpusCodecAPI] Fail to decode float pcm, error is -> ") + std::to_string(_errDecoder));
            return -1;
        }
    }
    else {
        DefaultLogger::Log(LogLevelWarn, std::string("[OpusCodecAPI] decoder not initialized"));
        return -1;
    }
}

void b12software::babel::OpusCodecAPI::initializeEncoder(int channel)
{
    if (isEncoderInitialized() == true) {
        DefaultLogger::Log(LogLevelInfo, std::string("[OpusCodecAPI] Encoder already initialize."));
        return;
    }
    _encoder = opus_encoder_create(_samplingRate, channel, OPUS_APPLICATION_VOIP, &_errEncoder);
    if (_errEncoder == OPUS_OK) {
        opus_encoder_init(_encoder, _samplingRate, _nbChannels, OPUS_APPLICATION_VOIP);
        opus_encoder_ctl(_encoder, OPUS_SET_BITRATE(_bitRate));
        _encoderInitialized = true;
        DefaultLogger::Log(LogLevelDebug, std::string("[OpusCodecAPI] Create Opus Encoder."));
        return;
    }
    else {
        DefaultLogger::Log(LogLevelError, std::string("[OpusCodecAPI] Error during encoder creation -> ") + std::to_string(_errEncoder));
        return;
    }
}

void b12software::babel::OpusCodecAPI::initializeDecoder(int channel)
{
    if (isDecoderInitialized() == true) {
        DefaultLogger::Log(LogLevelInfo, std::string("[OpusCodecAPI] Decoder already initialize."));
        return;
    }
    _decoder = opus_decoder_create(_samplingRate, channel, &_errDecoder);
    if (_errDecoder == OPUS_OK) {
        opus_decoder_init(_decoder, _samplingRate, _nbChannels);
        opus_decoder_ctl(_decoder, OPUS_SET_BITRATE(_bitRate));
        _decoderInitialized = true;
        DefaultLogger::Log(LogLevelDebug, std::string("[OpusCodecAPI] Create Opus Decoder."));
        return;
    }
    else {
        DefaultLogger::Log(LogLevelError, std::string("[OpusCodecAPI] Error during decoder creation -> ") + std::to_string(_errDecoder));
        return;
    }
}