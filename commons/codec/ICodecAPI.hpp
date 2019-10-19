/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** ICodecAPI.hpp
*/

/* Created the 17/09/2019 at 17:41 by julian.frabel@epitech.eu */

#ifndef BABEL_ICODECAPI_HPP
#define BABEL_ICODECAPI_HPP

#include <cstdint>
#include <cstddef>

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
         * @class ICodecAPI
         * @brief Interface for codec operations
         *
         * If the children of this class must throw an exception it should be a child of CodecAPIException
         */
        class ICodecAPI {
        public:
            /*!
             * @brief dtor
             */
            virtual ~ICodecAPI() = default;

            /*!
             * @brief Set the bit rate to use
             * @param bitRate The bit rate to use
             */
            virtual void setBitRate(std::int32_t bitRate) = 0;
            /*!
             * @brief Set the complexity to use
             * @param bitRate The complexity to use
             */
            virtual void setComplexity(std::int32_t complexity) = 0;
            /*!
             * @brief Set the number of channel to use
             * @param nbChannel the number of channel to use
             */
            virtual void setNbChannels(size_t nbChannel) = 0;
            /*!
             * @brief Set the sampling rate to use
             * @param samplingRate The sampling rate to use
             */
            virtual void setSamplingRate(std::int32_t samplingRate) = 0;

            /*!
             * @brief Get the bit rate to use
             * @return The bit rate to use
             */
            [[nodiscard]] virtual std::int32_t getBitRate() const = 0;
            /*!
             * @brief Get the complexity to use
             * @return The complexity to use
             */
            [[nodiscard]] virtual std::int32_t getComplexity() const = 0;
            /*!
             * @brief Get the number of channels to use
             * @return The number of channel to use
             */
            [[nodiscard]] virtual size_t getNbChannels() const = 0;
            /*!
             * @brief Get the sampling rate to use
             * @return The sampling rate to use
             */
            [[nodiscard]] virtual std::int32_t getSamplingRate() const = 0;

            /*!
             * @brief Encode a signal
             * @param pcm The input signal to encode
             * @param frameSize The size of a frame in the input signal
             * @param data Output buffer, should be at least of the size dataSize
             * @param dataSize Size of the allocated memory for the output buffer
             */
            virtual int encode(const std::int16_t *pcm, int frameSize, unsigned char *data, size_t dataSize) = 0;
            /*!
             * @brief Encode a signal
             * @param pcm The input signal to encode
             * @param frameSize The size of a frame in the input signal
             * @param data Output buffer, should be at least of the size dataSize
             * @param dataSize Size of the allocated memory for the output buffer
             */
            virtual int encode(const float *pcm, int frameSize, unsigned char *data, size_t dataSize) = 0;

            /*!
             * @brief Decode a signal
             * @param data Input encoded data, should be of len dataSize
             * @param dataSize Number of bytes in the data array
             * @param pcm The output signal to decode to
             * @param frameSize The size of a frame in the output signal
             */
            virtual int decode(const unsigned char *data, size_t dataSize, std::int16_t *pcm, int frameSize) = 0;
            /*!
             * @brief Decode a signal
             * @param data Input encoded data, should be of len dataSize
             * @param dataSize Number of bytes in the data array
             * @param pcm The output signal to decode to
             * @param frameSize The size of a frame in the output signal
             */
            virtual int decode(const unsigned char *data, size_t dataSize, float *pcm, int frameSize) = 0;
        };
    }
}
#endif //BABEL_ICODECAPI_HPP
