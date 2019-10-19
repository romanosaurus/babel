/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** PortAudioApi.cpp
*/

/* Created the 18/09/2019 at 11:35 by julian.frabel@epitech.eu */

#include <cstring>
#include <algorithm>
#include "logger/DefaultLogger.hpp"
#include "PortAudioAPI.hpp"
#include "PortAudioAPIException.hpp"

bool b12software::babel::PortAudioAPI::initialized = false;
size_t b12software::babel::PortAudioAPI::nbInstances = 0;

b12software::babel::PortAudioAPI::PortAudioAPI()
	: _nbInputChannels(0), _nbOutputChannels(0), _sampleRate(44100),
	_inputMuted(false), _outputMuted(false),
	_currentInputDeviceName(""), _currentInputDeviceIndex(0),
	_currentOutputDeviceName(""), _currentOutputDeviceIndex(0),
	_inputStream(nullptr), _outputStream(nullptr),
	_inputBuffer(nullptr), _outputBuffer(nullptr),
	_alignedInputBuffer(nullptr), _alignedOutputBuffer(nullptr)
{
    InitializeAPI();
    setToDefaultDevices();
    reloadCurrentDeviceInfos();
    reloadAudioAPI();
}

b12software::babel::PortAudioAPI::PortAudioAPI(size_t nbInputChannel, size_t nbOutputChannel)
    : _nbInputChannels(0), _nbOutputChannels(0), _sampleRate(44100),
      _inputMuted(false), _outputMuted(false),
      _currentInputDeviceName(""), _currentInputDeviceIndex(0),
      _currentOutputDeviceName(""), _currentOutputDeviceIndex(0),
      _inputStream(nullptr), _outputStream(nullptr),
      _inputBuffer(nullptr), _outputBuffer(nullptr),
	  _alignedInputBuffer(nullptr), _alignedOutputBuffer(nullptr)
{
    InitializeAPI();
    setToDefaultDevices(nbInputChannel, nbOutputChannel);
    reloadCurrentDeviceInfos();
    reloadAudioAPI();
}

b12software::babel::PortAudioAPI::PortAudioAPI(const std::string &inputDevice, size_t nbInputChannel,
                                               const std::string &outputDevice, size_t nbOutputChannel,
                                               double sampleRate)
    : _nbInputChannels(nbInputChannel), _nbOutputChannels(nbOutputChannel), _sampleRate(sampleRate),
      _inputMuted(false), _outputMuted(false),
      _currentInputDeviceName(inputDevice), _currentInputDeviceIndex(0),
      _currentOutputDeviceName(outputDevice), _currentOutputDeviceIndex(0),
      _inputStream(nullptr), _outputStream(nullptr),
      _inputBuffer(nullptr), _outputBuffer(nullptr),
	  _alignedInputBuffer(nullptr), _alignedOutputBuffer(nullptr)
{
    InitializeAPI();
    reloadCurrentDeviceInfos();
    reloadAudioAPI();
}

b12software::babel::PortAudioAPI::~PortAudioAPI()
{
    if (_inputStream != nullptr) {
        PaError err = Pa_AbortStream(_inputStream);
        if (err != paNoError) {
            DefaultLogger::Log(LogLevelError, std::string("[PortAudioAPI] Failed to abort input stream: ") + Pa_GetErrorText(err));
        }
        err = Pa_CloseStream(_inputStream);
        if (err != paNoError) {
            DefaultLogger::Log(LogLevelError, std::string("[PortAudioAPI] Failed to close input stream: ") + Pa_GetErrorText(err));
        }
    }
    if (_outputStream != nullptr) {
        PaError err = Pa_AbortStream(_outputStream);
        if (err != paNoError) {
            DefaultLogger::Log(LogLevelError, std::string("[PortAudioAPI] Failed to abort output stream: ") + Pa_GetErrorText(err));
        }
        err = Pa_CloseStream(_outputStream);
        if (err != paNoError) {
            DefaultLogger::Log(LogLevelError, std::string("[PortAudioAPI] Failed to close output stream: ") + Pa_GetErrorText(err));
        }
    }
    try {
        TerminateAPI();
    } catch (PortAudioAPIException &e) {
        DefaultLogger::Log(LogLevelError, e.what());
    }
    delete _inputBuffer;
    delete _outputBuffer;
    delete [] _alignedInputBuffer;
    delete [] _alignedOutputBuffer;
}

void b12software::babel::PortAudioAPI::setNbInputChannels(size_t nbChannel)
{
    if (_nbInputChannels == nbChannel)
        return;
    _nbInputChannels = nbChannel;
    DefaultLogger::Log(LogLevelDebug, std::string("[PortAudioAPI] Number of input channels set to ") + std::to_string(_nbInputChannels));
    reloadAudioAPI();
}

void b12software::babel::PortAudioAPI::setNbOutputChannels(size_t nbChannel)
{
    if (_nbOutputChannels == nbChannel)
        return;
    _nbOutputChannels = nbChannel;
    DefaultLogger::Log(LogLevelDebug, std::string("[PortAudioAPI] Number of output channels set to ") + std::to_string(_nbOutputChannels));
    reloadAudioAPI();
}

void b12software::babel::PortAudioAPI::setSampleRate(double sampleRate)
{
    if (_sampleRate == sampleRate)
        return;
    _sampleRate = sampleRate;
    DefaultLogger::Log(LogLevelDebug, std::string("[PortAudioAPI] Sample rate set to ") + std::to_string(_sampleRate));
    reloadAudioAPI();
}

void b12software::babel::PortAudioAPI::setInputMuted(bool muted)
{
    if (_inputMuted == muted)
        return;
    _inputMuted = muted;
    DefaultLogger::Log(LogLevelDebug, std::string("[PortAudioAPI] Input ") + (_inputMuted ? "muted" : "unmuted"));
}

void b12software::babel::PortAudioAPI::setOutputMuted(bool muted)
{
    if (_outputMuted == muted)
        return;
    _outputMuted = muted;
    DefaultLogger::Log(LogLevelDebug, std::string("[PortAudioAPI] Output ") + (_outputMuted ? "muted" : "unmuted"));
}

size_t b12software::babel::PortAudioAPI::getNbInputChannels() const
{
    return _nbInputChannels;
}

size_t b12software::babel::PortAudioAPI::getNbOutputChannels() const
{
    return _nbOutputChannels;
}

double b12software::babel::PortAudioAPI::getSampleRate() const
{
    return _sampleRate;
}

bool b12software::babel::PortAudioAPI::isInputMuted() const
{
    return _inputMuted;
}

bool b12software::babel::PortAudioAPI::isOutputMuted() const
{
    return _outputMuted;
}

std::vector<const PaDeviceInfo *> b12software::babel::PortAudioAPI::getAvailableDevicesInfos() const
{
    std::vector<const PaDeviceInfo *> availableDevices;
    int nbDevices = Pa_GetDeviceCount();
    if (nbDevices < 0) {
        throw PortAudioAPIException(std::string("[PortAudioAPI] Failed to retrieve number of devices (Pa_GetDeviceCount returned ") + std::to_string(nbDevices) + ")", WHERE);
    }
    for (int i = 0; i < nbDevices; i++) {
        availableDevices.push_back(Pa_GetDeviceInfo(i));
    }
    return availableDevices;
}

std::vector<std::string> b12software::babel::PortAudioAPI::getAvailableDevices() const
{
    auto devicesInfos = getAvailableDevicesInfos();
    std::vector<std::string> availableDevices;
    for (auto &info : devicesInfos) {
        availableDevices.emplace_back(info->name);
    }
    return availableDevices;
}

void b12software::babel::PortAudioAPI::setInputDevice(const std::string &device)
{
    auto availableDevices = getAvailableDevicesInfos();
    if (std::find_if(availableDevices.begin(), availableDevices.end(), [device](const PaDeviceInfo *&deviceInfo) {
        return deviceInfo->name == device;
    }) == availableDevices.end()) {
        throw PortAudioAPIException(std::string("[PortAudioAPI] Unknown device ") + device, WHERE);
    }
    DefaultLogger::Log(LogLevelDebug, "[PortAudioAPI] Changed Port Audio Input Device to " + device);
    _currentInputDeviceName = device;
    reloadCurrentDeviceInfos();
    reloadAudioAPI();
}

const std::string &b12software::babel::PortAudioAPI::getCurrentInputDevice() const
{
    return _currentInputDeviceName;
}

void b12software::babel::PortAudioAPI::setOutputDevice(const std::string &device)
{
    auto availableDevices = getAvailableDevicesInfos();
    if (std::find_if(availableDevices.begin(), availableDevices.end(), [device](const PaDeviceInfo *&deviceInfo) {
        return deviceInfo->name == device;
    }) == availableDevices.end()) {
        throw PortAudioAPIException(std::string("[PortAudioAPI] Unknown device ") + device, WHERE);
    }
    DefaultLogger::Log(LogLevelDebug, "[PortAudioAPI] Changed Port Audio Output Device to " + device);
    _currentOutputDeviceName = device;
    reloadCurrentDeviceInfos();
    reloadAudioAPI();
}

const std::string &b12software::babel::PortAudioAPI::getCurrentOutputDevice() const
{
    return _currentOutputDeviceName;
}

size_t b12software::babel::PortAudioAPI::getMaxInputChannelsForDevice(const std::string &device) const
{
    auto availableDevices = getAvailableDevicesInfos();
    auto infos = std::find_if(availableDevices.begin(), availableDevices.end(), [device](const PaDeviceInfo *&deviceInfo) {
        return deviceInfo->name == device;
    });
    if (infos == availableDevices.end()) {
        throw PortAudioAPIException(std::string("[PortAudioAPI] Unknown device ") + device, WHERE);
    }
    return (*infos)->maxInputChannels;
}

size_t b12software::babel::PortAudioAPI::getMaxOutputChannelsForDevice(const std::string &device) const
{
    auto availableDevices = getAvailableDevicesInfos();
    auto infos = std::find_if(availableDevices.begin(), availableDevices.end(), [device](const PaDeviceInfo *&deviceInfo) {
        return deviceInfo->name == device;
    });
    if (infos == availableDevices.end()) {
        throw PortAudioAPIException(std::string("[PortAudioAPI] Unknown device ") + device, WHERE);
    }
    return (*infos)->maxOutputChannels;
}

double b12software::babel::PortAudioAPI::getDefaultSampleRateForDevice(const std::string &device) const
{
    auto availableDevices = getAvailableDevicesInfos();
    auto infos = std::find_if(availableDevices.begin(), availableDevices.end(), [device](const PaDeviceInfo *&deviceInfo) {
        return deviceInfo->name == device;
    });
    if (infos == availableDevices.end()) {
        throw PortAudioAPIException(std::string("[PortAudioAPI] Unknown device ") + device, WHERE);
    }
    return (*infos)->defaultSampleRate;
}

size_t b12software::babel::PortAudioAPI::extractInputFrames(size_t nbFrames,
                                                            b12software::babel::IAudioAPI::Sample *sampleBuffer)
{
    size_t inputFrameSize = getInputFrameSize();
    if (nbFrames == 0 || inputFrameSize == 0 || _inputBuffer == nullptr || sampleBuffer == nullptr)
        return 0;
    return _inputBuffer->read(sampleBuffer, nbFrames * inputFrameSize) / inputFrameSize;
}

size_t b12software::babel::PortAudioAPI::insertOutputFrames(size_t nbFrames,
                                                            const b12software::babel::IAudioAPI::Sample *sampleBuffer)
{
    size_t availableFrames = getNumberOfOutputBufferFreeFrames();
    if (nbFrames == 0 || _outputBuffer == nullptr || sampleBuffer == nullptr)
        return 0;
    if (availableFrames >= nbFrames) {
        _outputBuffer->write(sampleBuffer, nbFrames * getOutputFrameSize());
        return nbFrames;
    } else if (availableFrames > 0) {
        _outputBuffer->write(sampleBuffer, availableFrames * getOutputFrameSize());
        return availableFrames;
    }
    return 0;
}

size_t b12software::babel::PortAudioAPI::getNumberOfInputBufferedFrames() const
{
    auto inputFrameSize = getInputFrameSize();
    if (inputFrameSize && _inputBuffer != nullptr) {
        return _inputBuffer->getNbBufferedBytes() / inputFrameSize;
    }
    return 0;
}

size_t b12software::babel::PortAudioAPI::getNumberOfOutputBufferFreeFrames() const
{
    if (_outputBuffer == nullptr || getOutputFrameSize() == 0)
        return 0;
    size_t totalFrames = bufferSizeInSecond * _sampleRate;
    auto buffered = _outputBuffer->getNbBufferedBytes() / getOutputFrameSize();
    return totalFrames - buffered;
}

size_t b12software::babel::PortAudioAPI::getInputFrameSize() const
{
    return _nbInputChannels * sizeof(Sample);
}

size_t b12software::babel::PortAudioAPI::getOutputFrameSize() const
{
    return _nbOutputChannels * sizeof(Sample);
}

void b12software::babel::PortAudioAPI::setToDefaultDevices(size_t maxInputChannels, size_t maxOutputChannels)
{
    _currentInputDeviceIndex = Pa_GetDefaultInputDevice();
    const PaDeviceInfo *inputInfos = Pa_GetDeviceInfo(_currentInputDeviceIndex);
    _currentInputDeviceName = inputInfos->name;
    _nbInputChannels = inputInfos->maxInputChannels;
    if (_nbInputChannels > maxInputChannels)
        _nbInputChannels = maxInputChannels;

    _currentOutputDeviceIndex = Pa_GetDefaultOutputDevice();
    const PaDeviceInfo *outputInfos = Pa_GetDeviceInfo(_currentOutputDeviceIndex);
    _currentOutputDeviceName = outputInfos->name;
    _nbOutputChannels = outputInfos->maxOutputChannels;
    if (_nbOutputChannels > maxOutputChannels)
        _nbOutputChannels = maxOutputChannels;
}

void b12software::babel::PortAudioAPI::reloadCurrentDeviceInfos()
{
    auto availableDevices = getAvailableDevicesInfos();
    int nbDevices = availableDevices.size();
    for (int i = 0; i < nbDevices; i++) {
        if (availableDevices[i]->name == _currentInputDeviceName)
            _currentInputDeviceIndex = i;
        if (availableDevices[i]->name == _currentOutputDeviceName)
            _currentOutputDeviceIndex = i;
    }
}

void b12software::babel::PortAudioAPI::reloadAudioBuffers()
{
    int totalFrames = bufferSizeInSecond * _sampleRate;
    int nbInputSamples = totalFrames * _nbInputChannels;
    int nbOutputSamples = totalFrames * _nbOutputChannels;

    delete _inputBuffer;
    delete _outputBuffer;
	delete [] _alignedInputBuffer;
	delete [] _alignedOutputBuffer;
	_alignedInputBuffer = new Sample[framePerBuffer * _nbInputChannels];
	_alignedOutputBuffer = new Sample[framePerBuffer * _nbOutputChannels];
    if (nbInputSamples > 0)
        _inputBuffer = new b12software::buffer::CircularBuffer(nbInputSamples * sizeof(Sample));
    else
        _inputBuffer = nullptr;
    if (nbOutputSamples > 0)
        _outputBuffer = new b12software::buffer::CircularBuffer(nbOutputSamples * sizeof(Sample));
    else
        _outputBuffer = nullptr;
}

void b12software::babel::PortAudioAPI::reloadAudioStream()
{
    PaError err;

    PaStreamParameters inputParameters;
    inputParameters.device = _currentInputDeviceIndex;
    inputParameters.channelCount = _nbInputChannels;
    inputParameters.sampleFormat = sampleFormat;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;

    if (_inputStream) {
        err = Pa_AbortStream(_inputStream);
        if (err != paNoError) {
            throw PortAudioAPIException(std::string("[PortAudioAPI] Failed to abort input stream: ") + Pa_GetErrorText(err), WHERE);
        }
        err = Pa_CloseStream(_inputStream);
        if (err != paNoError) {
            throw PortAudioAPIException(std::string("[PortAudioAPI] Failed to close input stream: ") + Pa_GetErrorText(err), WHERE);
        }
    }

    _inputStream = nullptr;
    if (_nbInputChannels) {
        err = Pa_OpenStream(&_inputStream, &inputParameters, nullptr, _sampleRate, framePerBuffer, paNoFlag, PaInputCallback, this);
        if (err != paNoError) {
            throw PortAudioAPIException(std::string("[PortAudioAPI] Failed to open stream: ") + Pa_GetErrorText(err), WHERE);
        }
    }

    PaStreamParameters outputParameters;
    outputParameters.device = _currentOutputDeviceIndex;
    outputParameters.channelCount = _nbOutputChannels;
    outputParameters.sampleFormat = sampleFormat;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowInputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;

    if (_outputStream) {
        err = Pa_AbortStream(_outputStream);
        if (err != paNoError) {
            throw PortAudioAPIException(std::string("[PortAudioAPI] Failed to abort output stream: ") + Pa_GetErrorText(err), WHERE);
        }
        err = Pa_CloseStream(_outputStream);
        if (err != paNoError) {
            throw PortAudioAPIException(std::string("[PortAudioAPI] Failed to close output stream: ") + Pa_GetErrorText(err), WHERE);
        }
    }

    _outputStream = nullptr;
    if (_nbOutputChannels) {
        err = Pa_OpenStream(&_outputStream, nullptr, &outputParameters, _sampleRate, framePerBuffer, paNoFlag, PaOutputCallback, this);
        if (err != paNoError) {
            throw PortAudioAPIException(std::string("[PortAudioAPI] Failed to open stream: ") + Pa_GetErrorText(err), WHERE);
        }
    }
}

void b12software::babel::PortAudioAPI::reloadAudioAPI()
{
    DefaultLogger::Log(LogLevelDebug,
        std::string("[PortAudioAPI] Reloading audio streams (") +
        "sampleRate=" + std::to_string(_sampleRate) +
        ":inputDeviceName=" + _currentInputDeviceName +
        ":inputDeviceID=" + std::to_string(_currentInputDeviceIndex) +
        ":inputDeviceChannelAmount=" + std::to_string(_nbInputChannels) +
        ":outputDeviceName=" + _currentOutputDeviceName +
        ":outputDeviceID=" + std::to_string(_currentOutputDeviceIndex) +
        ":outputDeviceChannelAmount=" + std::to_string(_nbOutputChannels) +
        ")"
    );

    reloadAudioStream();
    reloadAudioBuffers();
    PaError err = Pa_StartStream(_inputStream);
    if (err != paNoError) {
        throw PortAudioAPIException(std::string("[PortAudioAPI] Failed to start input stream: ") + Pa_GetErrorText(err), WHERE);
    }
    err = Pa_StartStream(_outputStream);
    if (err != paNoError) {
        throw PortAudioAPIException(std::string("[PortAudioAPI] Failed to start output stream: ") + Pa_GetErrorText(err), WHERE);
    }
}

/* STATICS */

bool b12software::babel::PortAudioAPI::IsInitialized()
{
    return PortAudioAPI::initialized;
}

void b12software::babel::PortAudioAPI::InitializeAPI()
{
    if (!IsInitialized()) {
        PaError err = Pa_Initialize();
        if (err != paNoError) {
            throw PortAudioAPIException(
                std::string("[PortAudioAPI] Failed to initialize port audio API: ") + Pa_GetErrorText(err),
                WHERE
            );
        }
        PortAudioAPI::initialized = true;
        DefaultLogger::Log(LogLevelDebug, "[PortAudioAPI] Initialized Port Audio API");
    }
    PortAudioAPI::nbInstances++;
    DefaultLogger::Log(LogLevelDebug, "[PortAudioAPI] Created Port Audio API (" + std::to_string(PortAudioAPI::nbInstances) + " instances)");
}

void b12software::babel::PortAudioAPI::TerminateAPI()
{
    if (!IsInitialized()) {
        throw PortAudioAPIException(
            std::string("[PortAudioAPI] Wow this should never be throwed contact julian"),
            WHERE
        );
    }
    PortAudioAPI::nbInstances--;
    DefaultLogger::Log(LogLevelDebug, "[PortAudioAPI] Deleted Port Audio API (" + std::to_string(PortAudioAPI::nbInstances) + " instances)");
    if (PortAudioAPI::nbInstances == 0) {
        PaError err = Pa_Terminate();
        if (err != paNoError) {
            throw PortAudioAPIException(
                std::string("[PortAudioAPI] Failed to terminate port audio API: ") + Pa_GetErrorText(err),
                WHERE
            );
        }
        PortAudioAPI::initialized = false;
        DefaultLogger::Log(LogLevelDebug, "[PortAudioAPI] Terminated Port Audio API");
    }
}

int b12software::babel::PortAudioAPI::PaInputCallback(const void *input, void *output, unsigned long frameCount,
                                                 const PaStreamCallbackTimeInfo *timeInfo,
                                                 PaStreamCallbackFlags statusFlags, void *userData)
{
    (void)timeInfo;
    (void)statusFlags;
    (void)output;
	if (frameCount != framePerBuffer)
		DefaultLogger::Log(LogLevelWarn, "[PortAudioAPI] Expected " + std::to_string(framePerBuffer) + " frames per buffer but got " + std::to_string(frameCount));
    PortAudioAPI *data = static_cast<PortAudioAPI *>(userData);
    const Sample *rdata = static_cast<const Sample *>(input);
    BufferInput(rdata, data);
    return paContinue;
}

int b12software::babel::PortAudioAPI::PaOutputCallback(const void *input, void *output, unsigned long frameCount,
                                                      const PaStreamCallbackTimeInfo *timeInfo,
                                                      PaStreamCallbackFlags statusFlags, void *userData)
{
    (void)timeInfo;
    (void)statusFlags;
    (void)input;
	if (frameCount != framePerBuffer)
		DefaultLogger::Log(LogLevelWarn, "[PortAudioAPI] Expected " + std::to_string(framePerBuffer) + " frames per buffer but got " + std::to_string(frameCount));
    PortAudioAPI *data = static_cast<PortAudioAPI *>(userData);
    Sample *wdata = static_cast<Sample *>(output);
    OutputBuffer(wdata, data);
    return paContinue;
}

void b12software::babel::PortAudioAPI::BufferInput(const b12software::babel::IAudioAPI::Sample *input, b12software::babel::PortAudioAPI *api)
{
    if (api->_inputBuffer == nullptr)
        return;
    size_t inputFrameSize = api->getInputFrameSize();
    if (input == nullptr || api->_inputMuted) {
        std::memset(api->_alignedInputBuffer, silence, framePerBuffer * inputFrameSize);
        if (api->_inputBuffer->write(api->_alignedInputBuffer, framePerBuffer * inputFrameSize)) {
            DefaultLogger::Log(LogLevelWarn, "[PortAudioAPI] Some audio frames were lost due to input buffer beeing full");
        }
    } else {
        if (api->_inputBuffer->write(input, framePerBuffer * inputFrameSize)) {
            DefaultLogger::Log(LogLevelWarn, "[PortAudioAPI] Some audio frames were lost due to input buffer beeing full");
        }
    }

}

void b12software::babel::PortAudioAPI::OutputBuffer(b12software::babel::IAudioAPI::Sample *output, b12software::babel::PortAudioAPI *api)
{
    if (api->_outputBuffer == nullptr)
        return;
    size_t outputFrameSize = api->getOutputFrameSize();
    if (api->_outputBuffer->getNbBufferedBytes() == 0 || api->_outputMuted) {
        std::memset(output, silence, framePerBuffer * outputFrameSize);
        return;
    }
    size_t bytesBuffered = api->_outputBuffer->getNbBufferedBytes();
    size_t bytesToRead = (bytesBuffered > framePerBuffer * outputFrameSize) ? framePerBuffer * outputFrameSize : bytesBuffered;
    size_t bytesRead = api->_outputBuffer->read(api->_alignedOutputBuffer, bytesToRead);
    std::memset(reinterpret_cast<void *>(reinterpret_cast<intptr_t>(api->_alignedOutputBuffer) + bytesRead), silence, framePerBuffer * outputFrameSize - bytesRead);
    std::memcpy(output, api->_alignedOutputBuffer, framePerBuffer * outputFrameSize);
}
