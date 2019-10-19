/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** BabelDatagram.hpp
*/

/* Created the 03/10/2019 at 11:24 by julian.frabel@epitech.eu */

#ifndef BABEL_BABELDATAGRAM_HPP
#define BABEL_BABELDATAGRAM_HPP

#include "network/Datagram.hpp"
#include "audio/IAudioAPI.hpp"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <ws2tcpip.h>
    #include <winsock2.h>

    #define HTONS(x) (htons(x))
    #define NTOHS(x) (ntohs(x))

    #define HTONL(x) (htonl(x))
    #define NTOHL(x) (ntohl(x))

    #define HTONLL(x) (htonll(x))
    #define NTOHLL(x) (ntohll(x))
#else
    #include <sys/types.h>
    #include <netinet/in.h>

    #define HTONS(x) (htons(x))
    #define NTOHS(x) (ntohs(x))

    #define HTONL(x) (htonl(x))
    #define NTOHL(x) (ntohl(x))

    #define HTONLL(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
    #define NTOHLL(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))
#endif

/*!
 * @namespace b12software
 * @brief Main namespace for all b12 software
 */
namespace b12software {

    /*!
     * @namespace b12software::babel
     * @brief Main namespace for networking stuff
     */
    namespace babel {

        /*!
         * @class BabelDatagram
         * @brief A datagram for the babel protocol
         */
        class BabelDatagram : public network::Datagram {
        public:
            /*!
             * @brief ctor
             */
            BabelDatagram() = default;
            /*!
             * @brief dtor
             */
            ~BabelDatagram() = default;

        public:
            /*!
             * @brief Create a datagram for the babel protocol
             * @param userId The user id in communication with
             * @param timestamp The timestamp of this audio packet (used to discard late/duplicate packets)
             * @param audioDataSize Size of the audio array
             * @param audio Audio array of size audioDataSize
             */
            void createBabelAudioDatagram(long userId, long timestamp, size_t audioDataSize, const char *compressedAudio);

            /*!
             * @brief Extract data from a datagram for the babel protocol
             * @param userId The user id in communication with
             * @param timestamp The timestamp of this audio packet (used to discard late/duplicate packets)
             * @param audioDataSize Size of the audio array
             * @param audio Audio array of size audioDataSize
             *
             * The audio array will be allocated with new[] it's up to the caller to delete this data
             */
            void extractBabelAudioDatagram(long &userId, long &timestamp, size_t &audioDataSize, char *&compressedAudio);
        };
    }
}

#endif //BABEL_BABELDATAGRAM_HPP
