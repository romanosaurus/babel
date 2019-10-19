/*
** EPITECH PROJECT, 2019
** babel-v1
** File description:
** testPortAudioAPI
*/

/* Created the 04/10/2019 at 15:38 by charlie.jeanneau@epitech.eu */

#include <criterion/criterion.h>
#include "audio/PortAudioAPI.hpp"

static void init()
{}

static void fini()
{}

Test(test_portAudioAPI, get_set,
        .timeout=10,
        .description="Test the PortAudioAPI geter and seter",
        .init=init,
        .fini=fini
)
{
    bool inputMuted = true;
    bool outputMuted = true;    
    size_t inputChannels = 2;
    size_t outputChannels = 2;
    double samplingRate = 48000;

    b12software::babel::PortAudioAPI audioAPI;

    audioAPI.setInputMuted(inputMuted);
    audioAPI.setNbInputChannels(inputChannels);
    audioAPI.setNbOutputChannels(outputChannels);
    audioAPI.setOutputMuted(outputMuted);
    audioAPI.setSampleRate(samplingRate);

    cr_assert_eq(audioAPI.getNbInputChannels(), 2);
    cr_assert_eq(audioAPI.getNbOutputChannels(), 2);
    cr_assert_eq(audioAPI.getSampleRate(), 48000);
    cr_assert_eq(audioAPI.isInputMuted(), true);
    cr_assert_eq(audioAPI.isOutputMuted(), true);
}