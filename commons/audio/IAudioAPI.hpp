/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** IAudioLibrary.hpp
*/

/* Created the 17/09/2019 at 17:00 by julian.frabel@epitech.eu */

#ifndef BABEL_IAUDIOAPI_HPP
#define BABEL_IAUDIOAPI_HPP

#include <vector>

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
         * @class IAudioAPI
         * @brief Interface for audio I/O operations
         *
         * If the children of this class must throw an exception it should be a child of AudioAPIException
         */
        class IAudioAPI {
        public:
            using Sample = float; /*!< The sample format */

        public:
            /*!
             * @brief dtor
             */
            virtual ~IAudioAPI() = default;

            /*!
             * @brief Set the number of input channel to use
             * @param nbChannel the number of input channel to use
             */
            virtual void setNbInputChannels(size_t nbChannel) = 0;
            /*!
             * @brief Set the number of output channel to use
             * @param nbChannel the number of output channel to use
             */
            virtual void setNbOutputChannels(size_t nbChannel) = 0;
            /*!
             * @brief Set the sample rate to use
             * @param sampleRate the sample rate to use
             */
            virtual void setSampleRate(double sampleRate) = 0;
            /*!
             * @brief Mute or unmute the input audio stream
             * @param muted should the input stream be muted
             */
            virtual void setInputMuted(bool muted) = 0;
            /*!
             * @brief Mute or unmute the output audio stream
             * @param muted should the output stream be muted
             */
            virtual void setOutputMuted(bool muted) = 0;

            /*!
             * @brief Get the number of input channel to use
             * @return the number of input channel to use
             */
            [[nodiscard]] virtual size_t getNbInputChannels() const = 0;
            /*!
             * @brief Get the number of output channel to use
             * @return the number of output channel to use
             */
            [[nodiscard]] virtual size_t getNbOutputChannels() const = 0;
            /*!
             * @brief Get the sample rate to use
             * @return the sample rate to use
             */
            [[nodiscard]] virtual double getSampleRate() const = 0;
            /*!
             * @brief Get if the input stream is muted
             * @return true if the input stream is muted false otherwise
             */
            [[nodiscard]] virtual bool isInputMuted() const = 0;
            /*!
             * @brief Get if the output stream is muted
             * @return true if the output stream is muted false otherwise
             */
            [[nodiscard]] virtual bool isOutputMuted() const = 0;

            /*!
             * @brief Get a vector of all available devices
             * @return A vector of available devices
             */
            [[nodiscard]] virtual std::vector<std::string> getAvailableDevices() const = 0;
            /*!
             * @brief Set the device to use for the input
             * @param device The device to use
             */
            virtual void setInputDevice(const std::string &device) = 0;
            /*!
             * @brief Set the device to use for the output
             * @param device The device to use
             */
            virtual void setOutputDevice(const std::string &device) = 0;
            /*!
              * @brief Get the device currently being used as input
              * @return The device being used
              */
            [[nodiscard]] virtual const std::string &getCurrentInputDevice() const = 0;
            /*!
              * @brief Get the device currently being used as output
              * @return The device being used
              */
            [[nodiscard]] virtual const std::string &getCurrentOutputDevice() const = 0;
            /*!
             * @brief Get the maximum available input channels for a device
             * @param device The device to get information from
             * @return the maximum available input channels
             */
            [[nodiscard]] virtual size_t getMaxInputChannelsForDevice(const std::string &device) const = 0;
            /*!
             * @brief Get the maximum available output channels for a device
             * @param device The device to get information from
             * @return the maximum available output channels
             */
            [[nodiscard]] virtual size_t getMaxOutputChannelsForDevice(const std::string &device) const = 0;
            /*!
             * @brief Get the default sample rate for a device
             * @param device The device to get information from
             * @return the default sample rate
             */
            [[nodiscard]] virtual double getDefaultSampleRateForDevice(const std::string &device) const = 0;

            /*!
             * @brief Extract input frames to sampleBuffer
             * @param nbFrames The number of audio frames to extract
             * @param sampleBuffer The buffer to extract the frames to (should be at least nbFrames * nbInputChannels * sizeof(Sample) bytes of size)
             * @return The number of frames that were extracted
             */
            [[nodiscard]] virtual size_t extractInputFrames(size_t nbFrames, Sample *sampleBuffer) = 0;
            /*!
             * @brief Insert output frames from sampleBuffer
             * @param nbFrames The number of frames to play
             * @param sampleBuffer The buffer to read the frames from (should be at least nbFrames * nbOutputChannels * sizeof(Sample) bytes of size)
             * @return The actual number of frames that were added to play
             */
            [[nodiscard]] virtual size_t insertOutputFrames(size_t nbFrames, const Sample *sampleBuffer) = 0;
            /*!
             * @brief Get the number of buffered frames for the input data
             * @return The number of buffered frames
             *
             * An input frame has a size of nbInputChannels * sizeof(Sample) bytes
             */
            [[nodiscard]] virtual size_t getNumberOfInputBufferedFrames() const = 0;
            /*!
             * @brief Get the number of frames that can fit in the output buffer
             * @return The number of free frames in the output buffer
             *
             * An output frame has a size of nbOutputChannels * sizeof(Sample) bytes
             */
            [[nodiscard]] virtual size_t getNumberOfOutputBufferFreeFrames() const = 0;
        };
    }
}

#endif //BABEL_IAUDIOAPI_HPP
