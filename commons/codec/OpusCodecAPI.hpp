/*
** EPITECH PROJECT, 2019
** babel
** File description:
** OpusCodecAPI.hpp
*/

/* Created the 23/09/2019 at 13:37 by charlie.jeanneau@epitech.eu */

#ifndef BABEL_OPUSCODECAPI_HPP
#define BABEL_OPUSCODECAPI_HPP

#include <opus/opus.h>
#include <cstdint>
#include "ICodecAPI.hpp"

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
         * @class CodecAPI
         * @brief OpusCodec implementation of the codec api
         */
        class OpusCodecAPI : public ICodecAPI {
        public:
            /*!
             * @brief ctor
             */
            OpusCodecAPI();
            /*!
             * @brief dtor
             */
            ~OpusCodecAPI() override;

        public:
            void setBitRate(std::int32_t bitRate) override;
            void setComplexity(std::int32_t complexity) override;
            void setNbChannels(size_t nbChannel) override;
            void setSamplingRate(std::int32_t samplingRate) override;

            [[nodiscard]] std::int32_t getBitRate() const override;
            [[nodiscard]] std::int32_t getComplexity() const override;
            [[nodiscard]] size_t getNbChannels() const override;
            [[nodiscard]] std::int32_t getSamplingRate() const override;

        private:
            std::int32_t _bitRate; /*!< Current bit Rate */
            std::int32_t _complexity; /*!< Current complexity */
            size_t _nbChannels; /*!< Current number of channels */
            std::int32_t _samplingRate; /*!< Current sampling rate */

        public:
            /*!
             * @brief Get information about Encoder initialisation
             * @return true if Encoder is initialized false otherwise
             */
            bool isEncoderInitialized();
            /*!
             * @brief Get information about Decoder initialisation
             * @return true if Decoder is initialized false otherwise
             */
            bool isDecoderInitialized();
            /*!
             * @brief Initialize Encoder
             */
            void initializeEncoder(int channel);
            /*
             * @brief Initialize Decoder
             */
            void initializeDecoder(int channel);
        private:
            bool _encoderInitialized; /*!< True if encoder initialized false otherwise */
            bool _decoderInitialized; /*!< True if decoder initialized false otherwise */
            OpusEncoder *_encoder; /*!< Holds the state of the encoder */
            OpusDecoder *_decoder; /*!< Holds the state of the decoder */
            int _errEncoder; /*!< Holds the error of the encoder */
            int _errDecoder; /*!< Holds the error of the decoder */
        
        public:
            int encode(const std::int16_t *pcm, int frameSize, unsigned char *data, size_t dataSize) override;
            int encode(const float *pcm, int frameSize, unsigned char *data, size_t dataSize) override;
            int decode(const unsigned char *data, size_t dataSize, std::int16_t *pcm, int frameSize) override;
            int decode(const unsigned char *data, size_t dataSize, float *pcm, int frameSize) override;

        };

    }

}

#endif //BABEL_OPUSCODECAPI_HPP