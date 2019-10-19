/*
** EPITECH PROJECT, 2019
** babel-v1
** File description:
** testsOpusCodecAPI
*/

/* Created the 04/10/2019 at 14:59 by charlie.jeanneau@epitech.eu */

#include <criterion/criterion.h>
#include "codec/OpusCodecAPI.hpp"

static void init()
{}

static void fini()
{}

Test(test_opusCodecAPI, get_set,
        .timeout=10,
        .description="Test the opusCodecAPI geter and seter",
        .init=init,
        .fini=fini
)
{
        std::uint32_t bitRate = 234;
        std::uint32_t complexity = 123;
        size_t nbChannels = 2;
        uint32_t samplingRate = 938;
        b12software::babel::OpusCodecAPI codecAPI;

        codecAPI.setBitRate(bitRate);
        codecAPI.setComplexity(complexity);
        codecAPI.setNbChannels(nbChannels);
        codecAPI.setSamplingRate(samplingRate);

        cr_assert_eq(codecAPI.getBitRate(), bitRate);
        cr_assert_eq(codecAPI.getComplexity(), complexity);
        cr_assert_eq(codecAPI.getNbChannels(), nbChannels);
        cr_assert_eq(codecAPI.getSamplingRate(), samplingRate);
}

Test(test_opusCodecAPI, init_encoder_decoder,
                .timeout=10,
                .description="Test the opusCodecAPI init encoder & decoder",
                .init=init,
                .fini=fini
)
{
        std::uint32_t bitRate = 64000;
        std::uint32_t complexity = 1;
        size_t nbChannels = 2;
        uint32_t samplingRate = 48000;
        b12software::babel::OpusCodecAPI codecAPI;

        cr_assert_eq(codecAPI.isEncoderInitialized(), false);
        cr_assert_eq(codecAPI.isDecoderInitialized(), false);

        codecAPI.initializeEncoder(2);
        codecAPI.initializeDecoder(2);

        cr_assert_eq(codecAPI.isEncoderInitialized(), false);
        cr_assert_eq(codecAPI.isDecoderInitialized(), false);

        codecAPI.setBitRate(bitRate);
        codecAPI.setComplexity(complexity);
        codecAPI.setNbChannels(nbChannels);
        codecAPI.setSamplingRate(samplingRate);

        codecAPI.initializeDecoder(2);
        codecAPI.initializeEncoder(2);

        cr_assert_eq(codecAPI.isDecoderInitialized(), true);
        cr_assert_eq(codecAPI.isEncoderInitialized(), true);
}