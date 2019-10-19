//
// Created by Brian on 01/10/2019.
//

#ifndef BABEL_QTNETWORKMANAGER_HPP
#define BABEL_QTNETWORKMANAGER_HPP

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
        class QtNetworkManager : public INetworkManager {
        public:
            /*!
             * @brief ctor
             */
            QtNetworkManager();
            /*!
             * @brief dtor
             */
            ~QtNetworkManager() override;
            void start() override;
            void terminate() override;
            IUdpSocket *createNewUdpSocket() override;
            ITcpClient *createNewTcpClient() override;
            ITcpServer *createNewTcpServer() override;
            void destroyUdpSocket(IUdpSocket *socket) override;
            void destroyTcpClient(ITcpClient *client) override;
            void destroyTcpServer(ITcpServer *server) override;

        private:
            std::vector<IUdpSocket *> _updSockets;
            std::vector<ITcpClient *> _tcpClients;
        };
    }
};

#endif //BABEL_QTNETWORKMANAGER_HPP
