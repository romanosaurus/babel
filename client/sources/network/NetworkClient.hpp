/*
** EPITECH PROJECT, 2019
** test
** File description:
** NetworkClient
*/

#ifndef BABEL_NETWORKCLIENT_HPP
#define BABEL_NETWORKCLIENT_HPP

#include "network/ITcpClient.hpp"
#include "QT/QtNetworkManager.hpp"
#include "network/HostInfos.hpp"
#include "network/NetworkException.hpp"
#include "network/IUdpSocket.hpp"
#include "logger/DefaultLogger.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>

/*!
 * @namespace b12software
 * @brief Main namespace for all b12 software
 */
namespace b12software {

    /*!
     * @namespace b12software::network
     * @brief Main namespace for the babel project
     */
    namespace network {

        /*!
         * @class NetworkClient
         * @brief NetworkClient implementation of the network for client
         */
        class NetworkClient {
    	public:
            /*!
            * @brief ctor
            */
    		NetworkClient();
            /*!
            * @brief dtor
            */
    		~NetworkClient();
            /*!
             * @brief Connect a client
             * @param hostInfo
             */
            void connectClient();
            /*!
             * @brief Disconnect a client
             */
            void disconnectClient();
            /*!
             * @brief run connected client
             */
            void run();

            std::shared_ptr<network::ITcpClient> getClient();

            std::shared_ptr<network::IUdpSocket> getUdpSocket();
    	protected:
    	private:
            network::INetworkManager *_networkManager;
            std::shared_ptr<network::ITcpClient> _client;
            std::shared_ptr<network::IUdpSocket> _udpSocket;
        };
    }
}

#endif // BABEL_NETWORKCLIENT_HPP
