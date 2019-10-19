/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** ITcpClient.hpp
*/

/* Created the 24/09/2019 at 20:16 by julian.frabel@epitech.eu */

#ifndef BABEL_ITCPCLIENT_HPP
#define BABEL_ITCPCLIENT_HPP

#include "IPacket.hpp"
#include "HostInfos.hpp"

/*!
 * @namespace b12software
 * @brief Main namespace for all b12 software
 */
namespace b12software {

    /*!
     * @namespace b12software::network
     * @brief Main namespace for networking stuff
     */
    namespace network {

        /*!
         * @class ITcpClient
         * @brief An interface describing the use cases of a TCP client
         */
        class ITcpClient {
        public:
            /*!
             * @brief dtor
             */
            virtual ~ITcpClient() = default;

            /*!
             * @brief Connect to a specific host
             * @param infos The informations about the host to connect to
             */
            virtual void connect(const HostInfos &infos) = 0;
            /*!
             * @brief Disconnect from the connected host
             */
            virtual void disconnect() = 0;
            /*!
             * @brief Check if this client is connected to a server
             * @return true if the client is connected false otherwise
             */
            virtual bool isConnected() const = 0;
            /*!
             * @brief Get the information about the host this client is connected to
             * @return HostInfos of the connected host
             */
            virtual const HostInfos &getHostInfos() const = 0;
            /*!
             * @brief Get the information about this client
             * @return Information about this client
             */
            virtual const HostInfos &getSelfInfos() const = 0;

            /*!
             * @brief Send raw bytes to the connected host
             * @param data The data to send (must be of size size)
             * @param size The size of the data buffer
             * @return The size that was taken into account / sent
             */
            virtual size_t send(const void *data, size_t size) = 0;
            /*!
             * @brief Send a packet on the network
             * @param packet The packet to send
             *
             * The packet should be valid. If invalid this function should ignore it
             */
            virtual void send(const IPacket &packet) = 0;
            /*!
             * @brief Receive raw data from the network
             * @param data The buffer to copy the data to
             * @param size The size of the data buffer
             * @return The amount of bytes that were extracted to the data buffer
             */
            virtual size_t receive(void *data, size_t size) = 0;
            /*!
             * @brief Receive a packet from the network
             * @param packet The packet to push the data to
             *
             * The client will try to feed all available data to the packet.
             * It's up to the packet implementation to only get the required amount.
             * All data that are not accepted by the packet should be keeped in the client until next receive call.
             */
            virtual void receive(IPacket &packet) = 0;

            /*!
             * @brief Get the amount of buffered bytes in the client that are available to receive
             * @return The amount of available bytes
             */
            virtual size_t getAvailableBytesSize() const = 0;
        };
    }
}

#endif //BABEL_ITCPCLIENT_HPP
