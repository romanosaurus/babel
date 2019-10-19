/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** BabelDatagram.cpp
*/

/* Created the 03/10/2019 at 11:24 by julian.frabel@epitech.eu */

#include <cstring>
#include <iostream>
#include <logger/DefaultLogger.hpp>
#include "BabelDatagram.hpp"

void b12software::babel::BabelDatagram::createBabelAudioDatagram(long userId, long timestamp, size_t audioDataSize,
                                                                 const char *compressedAudio)
{
    size_t totSize = sizeof(userId) + sizeof(timestamp) + sizeof(audioDataSize) + audioDataSize * sizeof(*compressedAudio);
    unsigned char *tmpBuff = new unsigned char[totSize];
    intptr_t ptr = reinterpret_cast<intptr_t>(tmpBuff);

    userId = HTONLL(userId);
    timestamp = HTONLL(timestamp);
    size_t audioDataSizeNetwork = HTONLL(audioDataSize);
    std::memcpy(reinterpret_cast<void *>(ptr), &userId, sizeof(userId));
    std::memcpy(reinterpret_cast<void *>(ptr + sizeof(userId)), &timestamp, sizeof(timestamp));
    std::memcpy(reinterpret_cast<void *>(ptr + sizeof(userId) + sizeof(timestamp)), &audioDataSizeNetwork, sizeof(audioDataSize));
    std::memcpy(reinterpret_cast<void *>(ptr + sizeof(userId) + sizeof(timestamp) + sizeof(audioDataSize)), compressedAudio, audioDataSize * sizeof(*compressedAudio));
    setData(tmpBuff, totSize);
    delete [] tmpBuff;
}

void b12software::babel::BabelDatagram::extractBabelAudioDatagram(long &userId, long &timestamp, size_t &audioDataSize,
                                                                  char *&compressedAudio)
{
    size_t required = sizeof(userId) + sizeof(timestamp) + sizeof(audioDataSize);
    if (!isValid() || getDatagramSize() < required) {
        babel::DefaultLogger::Log(babel::LogLevelInfo, "[BabelDatagram] Failed to read datagram");
        return;
    }
    intptr_t ptr = reinterpret_cast<intptr_t>(getData());
    const long *userIdPtr = reinterpret_cast<const long *>(ptr);
    const long *timestampPtr = reinterpret_cast<const long *>(ptr + sizeof(userId));
    const size_t *dataSizePtr = reinterpret_cast<const size_t *>(ptr + sizeof(userId) + sizeof(timestamp));
    userId = NTOHLL(*userIdPtr);
    timestamp = NTOHLL(*timestampPtr);
    audioDataSize = NTOHLL(*dataSizePtr);
    required += audioDataSize;
    if (getDatagramSize() < required) {
        babel::DefaultLogger::Log(babel::LogLevelInfo, "[BabelDatagram] Failed to read datagram");
        return;
    }
    compressedAudio = new char[audioDataSize];
    std::memcpy(compressedAudio, reinterpret_cast<void *>(ptr + sizeof(userId) + sizeof(timestamp) + sizeof(audioDataSize)), audioDataSize * sizeof(*compressedAudio));
}
