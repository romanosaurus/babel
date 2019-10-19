/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** PortAudioApi.hpp
*/

/* Created the 18/09/2019 at 11:35 by julian.frabel@epitech.eu */

#ifndef BABEL_PORTAUDIOAPI_HPP
#define BABEL_PORTAUDIOAPI_HPP

#include <cstddef>
#include <cstring>
#include <limits>
#include <portaudio.h>
#include "b12software/buffer/CircularBuffer.hpp"
#include "IAudioAPI.hpp"


/*!
 * @namespace b12software
 * @brief Main namespace for all b12 software
 */
namespace b12software {

    /*!
     * @namespace b12software::babel
     * @brief Main namespace for the babel project
     */
    namespace babel {

        /*!
         * @class PortAudioAPI
         * @brief PortAudio implementation of the audio api
         */
        class PortAudioAPI : public IAudioAPI {
        public:
            /*!
             * @brief ctor
             */
            PortAudioAPI();

            /*!
             * @brief ctor that max the number of input and output channels to use
             * @param nbInputChannel The number of input channel to use
             * @param nbOutputChannel The number of output channels
             */
            PortAudioAPI(size_t nbInputChannel, size_t nbOutputChannel);

            /*!
             * @brief ctor
             * @param inputDevice The name of the input device to use
             * @param nbInputChannel The number of input channel to use
             * @param outputDevice The name of the output device to use
             * @param nbOutputChannel The number of output channels
             * @param sampleRate The sample rate to use
             */
            PortAudioAPI(const std::string &inputDevice, size_t nbInputChannel,
                         const std::string &outputDevice, size_t nbOutputChannel,
                         double sampleRate = 44100);

            /*!
             * @brief dtor
             */
            ~PortAudioAPI() override;

        public:
            void setNbInputChannels(size_t nbChannel) override;
            void setNbOutputChannels(size_t nbChannel) override;
            void setSampleRate(double sampleRate) override;
            void setInputMuted(bool muted) override;
            void setOutputMuted(bool muted) override;

            [[nodiscard]] size_t getNbInputChannels() const override;
            [[nodiscard]] size_t getNbOutputChannels() const override;
            [[nodiscard]] double getSampleRate() const override;
            [[nodiscard]] bool isInputMuted() const override;
            [[nodiscard]] bool isOutputMuted() const override;

            [[nodiscard]] std::vector<std::string> getAvailableDevices() const override;
            void setInputDevice(const std::string &device) override;
            void setOutputDevice(const std::string &device) override;
            [[nodiscard]] const std::string &getCurrentInputDevice() const override;
            [[nodiscard]] const std::string &getCurrentOutputDevice() const override;
            [[nodiscard]] size_t getMaxInputChannelsForDevice(const std::string &device) const override;
            [[nodiscard]] size_t getMaxOutputChannelsForDevice(const std::string &device) const override;
            [[nodiscard]] double getDefaultSampleRateForDevice(const std::string &device) const override;

            [[nodiscard]] size_t extractInputFrames(size_t nbFrames, Sample *sampleBuffer) override;
            [[nodiscard]] size_t insertOutputFrames(size_t nbFrames, const Sample *sampleBuffer) override;
            [[nodiscard]] size_t getNumberOfInputBufferedFrames() const override;
            [[nodiscard]] size_t getNumberOfOutputBufferFreeFrames() const override;

        private:
            /*!
             * @brief Get the size in bytes of an input frame
             * @return The size in bytes of a frame
             */
            [[nodiscard]] size_t getInputFrameSize() const;
            /*!
             * @brief Get the size in bytes of an output frame
             * @return The size in bytes of a frame
             */
            [[nodiscard]] size_t getOutputFrameSize() const;
            /*!
             * @brief Set this api to use the current default devices
             * @param maxInputChannels The maximum amount of input channels to use
             * @param maxOutputChannels The maximum amount of output channels to use
             */
            void setToDefaultDevices(size_t maxInputChannels = (std::numeric_limits<size_t>::max)(), size_t maxOutputChannels = (std::numeric_limits<size_t>::max)());
            /*!
             * @brief Reloads the current device name information
             */
            void reloadCurrentDeviceInfos();
            /*!
             * @brief Reload and reallocate the buffers
             */
            void reloadAudioBuffers();
            /*!
             * @brief Reload and reallocate the audio stream
             */
            void reloadAudioStream();
            /*!
             * @brief Reload the internal port audio stream
             */
            void reloadAudioAPI();
            /*!
             * @brief Get the available devices information
             * @return A vector of device information
             */
            [[nodiscard]] std::vector<const PaDeviceInfo *> getAvailableDevicesInfos() const;

        public:
            /*!
             * @brief Get information about PortAudio initialisation
             * @return true if PortAudio is initialized false otherwise
             */
            static bool IsInitialized();

        private:
            /*!
             * @brief Initialize port audio if needed
             */
            static void InitializeAPI();
            /*!
             * @brief Terminate port audio if needed
             */
            static void TerminateAPI();

            /*!
             * @brief Port audio callback function
             * @param input Array of interleaved samples or; if non-interleaved samples were requested using the paNonInterleaved sample format flag, an array of buffer pointers, one non-interleaved buffer for each channel
             * @param output Array of interleaved samples or; if non-interleaved samples were requested using the paNonInterleaved sample format flag, an array of buffer pointers, one non-interleaved buffer for each channel
             * @param frameCount The number of sample frames to be processed by the stream callback
             * @param timeInfo Timestamps indicating the ADC capture time of the first sample in the input buffer, the DAC output time of the first sample in the output buffer and the time the callback was invoked
             * @param statusFlags Flags indicating whether input and/or output buffers have been inserted or will be dropped to overcome underflow or overflow conditions
             * @param userData A PortAudioAPI pointer pointing to the concerned API
             * @return paContinue if the stream should continue invoking the callback, paComplete if the stream should stop invoking the callback and finish once all output samples have played, paAbort if the stream should stop invoking the callback and finish as soon as possible
             *
             * WARNING: remember that you should not called function that may take an unbounded amount of time or you will be faced with glitched audio
             */
            static int PaInputCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

            /*!
             * @brief Port audio callback function
             * @param input Array of interleaved samples or; if non-interleaved samples were requested using the paNonInterleaved sample format flag, an array of buffer pointers, one non-interleaved buffer for each channel
             * @param output Array of interleaved samples or; if non-interleaved samples were requested using the paNonInterleaved sample format flag, an array of buffer pointers, one non-interleaved buffer for each channel
             * @param frameCount The number of sample frames to be processed by the stream callback
             * @param timeInfo Timestamps indicating the ADC capture time of the first sample in the input buffer, the DAC output time of the first sample in the output buffer and the time the callback was invoked
             * @param statusFlags Flags indicating whether input and/or output buffers have been inserted or will be dropped to overcome underflow or overflow conditions
             * @param userData A PortAudioAPI pointer pointing to the concerned API
             * @return paContinue if the stream should continue invoking the callback, paComplete if the stream should stop invoking the callback and finish once all output samples have played, paAbort if the stream should stop invoking the callback and finish as soon as possible
             *
             * WARNING: remember that you should not called function that may take an unbounded amount of time or you will be faced with glitched audio
             */
            static int PaOutputCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

            /*!
             * @brief Buffer the input of port audio
             * @param input The port audio input buffer
             * @param api The api to buffer to
             *
             * If the input buffer does not have enough space it will erase the oldest data and warn the user in the default logger
             */
            static void BufferInput(const Sample *input, PortAudioAPI *api);
            /*!
             * @brief Flush frameCount frames from the output buffer to portaudio output
             * @param output The port audio output buffer
             * @param api The api to get frames from
             *
             * If Not enough frames are buffered in the output buffer will flush silence to port audio buffer
             */
            static void OutputBuffer(Sample *output, PortAudioAPI *api);

        private:
            static constexpr Sample silence = 0.0f; /*!< The value of silence */
            static constexpr PaSampleFormat sampleFormat = paFloat32; /*!< The port audio sample format */
            static constexpr double bufferSizeInSecond = 3; /*!< The number of seconds that can be buffered */
            static constexpr size_t framePerBuffer = 480; /*!< The number of frame in the port audio buffers DO NOT USE paFramesPerBufferUnspecified or set to 0 */

        private:
            size_t _nbInputChannels; /*!< Number of current input channels */
            size_t _nbOutputChannels; /*!< Number of current output channels */
            double _sampleRate; /*!< Current sample rate */
            bool _inputMuted; /*!< Value of the input mute (true if muted) */
            bool _outputMuted; /*!< Value of the output mute (true if muted) */
            std::string _currentInputDeviceName; /*!< Name of the current used input device */
            PaDeviceIndex _currentInputDeviceIndex; /*!< Index of the current input device used */
            std::string _currentOutputDeviceName; /*!< Name of the current used output device */
            PaDeviceIndex _currentOutputDeviceIndex; /*!< Index of the current output device used */

            PaStream *_inputStream; /*!< The port audio input stream for this device */
            PaStream *_outputStream; /*!< The port audio output stream for this device */
            b12software::buffer::CircularBuffer *_inputBuffer; /*!< The input audio buffer */
            b12software::buffer::CircularBuffer *_outputBuffer; /*!< The output audio buffer */
			Sample *_alignedInputBuffer; /*!< Buffer used internally in callback functions to align the input */
			Sample *_alignedOutputBuffer; /*!< Buffer used internally in callback functions to align the output */

        private:
            static bool initialized; /*!< True if port audio is initialized false otherwise */
            static size_t nbInstances; /*!< Number of PortAudioAPI instances (used to compute when PortAudio should be terminated) */
        };
    }
}

#endif //BABEL_PORTAUDIOAPI_HPP
