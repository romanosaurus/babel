/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** ITcpServer.hpp
*/

/* Created the 24/09/2019 at 20:16 by julian.frabel@epitech.eu */

#ifndef BABEL_ITCPSERVER_HPP
#define BABEL_ITCPSERVER_HPP

#include <vector>
#include "ITcpClient.hpp"

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
         * @class ITcpServer
         * @brief An interface describing the use cases of a TCP server
         */
        class ITcpServer {
        public:
            /*!
             * @brief dtor
             */
            virtual ~ITcpServer() = default;

            /*!
             * @brief Bind a server to a port
             * @param port The port to bind to
             */
            virtual void bind(int port) = 0;
            /*!
             * @brief Unbind the server from the port
             */
            virtual void unbind() = 0;

            /*!
             * @brief Disconnect a client
             * @param clientId The id of the client to disconnect
             */
            virtual void disconnect(int clientId) = 0;

            /*!
             * @brief Send raw data to all connected clients
             * @param data The data to send
             * @param size The size of the data
             */
            virtual void sendToAll(const void *data, size_t size) = 0;
            /*!
             * @brief Send a packet to all connected clients
             * @param packet The packet to send
             */
            virtual void sendToAll(const IPacket &packet) = 0;

            /*!
             * @brief Send raw data to a specific client
             * @param clientId The id of the client to send the data to
             * @param data The data to send
             * @param size The size of the data
             */
            virtual void sendToClient(int clientId, const void *data, size_t size) = 0;
            /*!
             * @brief Send a packet to a specific client
             * @param clientId The id of the client to send the data to
             * @param packet The packet to send
             */
            virtual void sendToClient(int clientId, const IPacket &packet) = 0;

            /*!
             * @brief Send raw data to all clients in the clientIds vector
             * @param clientIds The ids of the client that should receive the data
             * @param data The data to send
             * @param size The size of the data to send
             */
            virtual void sendToClients(const std::vector<int> &clientIds, const void *data, size_t size) = 0;
            /*!
             * @brief Send a packet to all clients in the clientIds vector
             * @param clientIds The ids of the clients that should receive the packet
             * @param packet The packet to send
             */
            virtual void sendToClients(const std::vector<int> &clientIds, const IPacket &packet) = 0;

            /*!
             * @brief Send raw data to all connected clients that are not in exceptIds
             * @param exceptIds The ids of the clients that should not receive the data
             * @param data The data to send
             * @param size The size of the data
             */
            virtual void sendExceptClients(const std::vector<int> &exceptIds, const void *data, size_t size) = 0;
            /*!
             * @brief Send a packet to all connected clients that are not in exceptIds
             * @param exceptIds The ids of the clients that should not receive the packet
             * @param packet The packet to send
             */
            virtual void sendExceptClients(const std::vector<int> &exceptIds, const IPacket &packet) = 0;

            /*!
             * @brief Get all connected client ids
             * @return The ids of all connected clients
             */
            virtual std::vector<int> getConnectedClientIds() = 0;
            /*!
             * @brief Get the ids of the new connected clients since this function last call
             * @return The ids of the new clients
             */
            virtual std::vector<int> getNewConnectedClientIds() = 0;
            /*!
             * @brief Get the ids of the disconnected clients since this function last call
             * @return The ids of the disconnected clients
             */
            virtual std::vector<int> getDisconnectedClientIds() = 0;

            /*!
             * @brief Get a client by it's id
             * @param clientId The id of the client
             * @return A pointer to the requested client (will be null if the id is not found)
             */
            virtual ITcpClient *getClientById(int clientId) = 0;
        };
    }
}

#endif //BABEL_ITCPSERVER_HPP
