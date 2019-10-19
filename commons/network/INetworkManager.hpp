/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** INetworkManager.hpp
*/

/* Created the 25/09/2019 at 20:13 by julian.frabel@epitech.eu */

#ifndef BABEL_INETWORKMANAGER_HPP
#define BABEL_INETWORKMANAGER_HPP

#include "IUdpSocket.hpp"
#include "ITcpClient.hpp"
#include "ITcpServer.hpp"

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
         * @class INetworkManager
         * @brief An interface describing the network manager
         */
        class INetworkManager {
        public:
            /*!
             * @brief dtor
             */
            virtual ~INetworkManager() = default;

            /*!
             * @brief Start this network manager
             */
            virtual void start() = 0;

            /*!
             * @brief Terminate this network manager
             */
            virtual void terminate() = 0;

            /*!
             * @brief Create a new IUdpSocket
             * @return A IUdpSocket
             */
            virtual IUdpSocket *createNewUdpSocket() = 0;
            /*!
             * @brief Create a new ITcpClient
             * @return A ITcpClient
             */
            virtual ITcpClient *createNewTcpClient() = 0;
            /*!
             * @brief Create a new ITcpServer
             * @return A ITcpServer
             */
            virtual ITcpServer *createNewTcpServer() = 0;

            /*!
             * @brief Destroy a managed IUdpSocket
             * @param socket the socket to destroy
             */
            virtual void destroyUdpSocket(IUdpSocket *socket) = 0;
            /*!
             * @brief Destroy a managed ITcpClient
             * @param client the client to destroy
             */
            virtual void destroyTcpClient(ITcpClient *client) = 0;
            /*!
             * @brief Destroy a managed ITcpServer
             * @param server the server to destroy
             */
            virtual void destroyTcpServer(ITcpServer *server) = 0;
        };
    }
}

#endif //BABEL_INETWORKMANAGER_HPP
