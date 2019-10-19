/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** AsioTcpServer.hpp
*/

/* Created the 26/09/2019 at 11:37 by julian.frabel@epitech.eu */

#ifndef BABEL_ASIOTCPSERVER_HPP
#define BABEL_ASIOTCPSERVER_HPP

#include <map>
#include <vector>
#include <queue>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "network/ITcpServer.hpp"
#include "network/asio/AsioTcpClient.hpp"

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
         * @class AsioTcpServer
         * @brief Boost asio implementation of a tcp server
         */
        class AsioTcpServer : public ITcpServer {
        private:
            using acceptor = boost::asio::ip::tcp::acceptor;
            using endpoint = boost::asio::ip::tcp::endpoint;
            using mutex = boost::recursive_mutex;
            using lock = mutex::scoped_lock;

        public:
            /*!
             * @brief ctor
             * @param context boost io context
             */
            AsioTcpServer(boost::asio::io_context &context);
            /*!
             * @brief dtor
             */
            ~AsioTcpServer() override;

        public:
            void bind(int port) override;
            void unbind() override;
            void disconnect(int clientId) override;
            void sendToAll(const void *data, size_t size) override;
            void sendToAll(const IPacket &packet) override;
            void sendToClient(int clientId, const void *data, size_t size) override;
            void sendToClient(int clientId, const IPacket &packet) override;
            void sendToClients(const std::vector<int> &clientIds, const void *data, size_t size) override;
            void sendToClients(const std::vector<int> &clientIds, const IPacket &packet) override;
            void sendExceptClients(const std::vector<int> &exceptIds, const void *data, size_t size) override;
            void sendExceptClients(const std::vector<int> &exceptIds, const IPacket &packet) override;
            std::vector<int> getConnectedClientIds() override;
            std::vector<int> getNewConnectedClientIds() override;
            std::vector<int> getDisconnectedClientIds() override;
            ITcpClient *getClientById(int clientId) override;

        private:
            /*!
             * @brief Compute disconnected clients
             */
            void recomputeDisconnected();
            /*!
             * @brief Start an async call to accept clients
             */
            void startAccept();

            /*!
             * @brief Handle a connection of a new client
             * @param client The new connected client to recompute data for
             * @param error Boost errors that could have happened
             */
            void handleAccept(boost::shared_ptr<AsioTcpClient> client, const boost::system::error_code& error);

        private:
            boost::asio::io_context &_context; /*!< The boost asio io context */
            acceptor _acceptor; /*!< The tcp acceptor of this server */
            std::map<int, boost::shared_ptr<AsioTcpClient>> _clients; /*!< The connected clients stored with there ids */
            mutex _clientMapMutex; /*!< A mutex locking the client map*/
            mutex _idVectorMutex; /*!< A mutex locking the ids vector */
            std::vector<int> _newIds; /*!< A list of newly connected ids */
            std::queue<int> _freeIds; /*!< A list of available ids */
            std::vector<int> _disconnectedIds; /*!< A list of newly disconnected ids */
            int _maxId; /*!< The biggest id in use */
        };
    }
}

#endif //BABEL_ASIOTCPSERVER_HPP
