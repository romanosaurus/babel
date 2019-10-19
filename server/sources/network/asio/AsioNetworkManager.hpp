/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** AsioNetworkManager.hpp
*/

/* Created the 26/09/2019 at 11:19 by julian.frabel@epitech.eu */

#ifndef BABEL_ASIONETWORKMANAGER_HPP
#define BABEL_ASIONETWORKMANAGER_HPP

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <vector>
#include "network/INetworkManager.hpp"

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
         * @class AsioNetworkManager
         * @brief A network class that manage boost asio networking
         */
        class AsioNetworkManager : public INetworkManager {
        public:
            /*!
             * @brief ctor
             */
            AsioNetworkManager();
            /*!
             * @brief dtor
             */
            ~AsioNetworkManager() override;

        public:
            void start() override;
            void terminate() override;
            IUdpSocket *createNewUdpSocket() override;
            ITcpClient *createNewTcpClient() override;
            ITcpServer *createNewTcpServer() override;
            void destroyUdpSocket(IUdpSocket *socket) override;
            void destroyTcpClient(ITcpClient *client) override;
            void destroyTcpServer(ITcpServer *server) override;

        private:
            /*!
             * @brief Function exceuted on the network thread
             */
            static void networkingThreadFunc(AsioNetworkManager *manager);

        private:
            boost::asio::io_context _ioContext; /*!< Boost io context for this network manager */
            boost::asio::io_context::work *_worker; /*!< Boost object used such that _ioContext.run() does not terminate for lack of work */
            std::vector<IUdpSocket *> _udpSockets; /*!< Managed udp sockets */
            std::vector<ITcpClient *> _tcpClients; /*!< Managed tcp clients */
            std::vector<ITcpServer *> _tcpServers; /*!< Managed tcp servers */
            boost::thread _networkThread; /*!< Network thread */
            bool _started; /*!< Is the network manager started */
        };
    }
}

#endif //BABEL_ASIONETWORKMANAGER_HPP
