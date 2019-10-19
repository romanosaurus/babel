/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** AsioUdpSocket.hpp
*/

/* Created the 26/09/2019 at 11:42 by julian.frabel@epitech.eu */

#ifndef BABEL_ASIOUDPSOCKET_HPP
#define BABEL_ASIOUDPSOCKET_HPP

#include <queue>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include "network/IUdpSocket.hpp"

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
         * @class AsioUdpSocket
         * @brief Boost asio implementation of a udp socket
         */
        class AsioUdpSocket : public IUdpSocket {
        public:
            /*!
             * @brief ctor
             */
            explicit AsioUdpSocket(boost::asio::io_context &context);
            /*!
             * @brief dtor
             */
            ~AsioUdpSocket() override;

        public:
            void bind(int port) override;
            void unbind() override;
            void registerHost(const HostInfos &infos) override;
            void unregisterHost() override;
            void send(const void *data, size_t size, const HostInfos &infos) override;
            void send(const Datagram &datagram) override;
            Datagram receive() override;
            bool hasPendingDatagrams() const override;
            int getPort() const override;
            std::string getAddress() const override;

        private:
            static constexpr size_t maxReceiveDatagramSize = 1024; /*!< The maximum allowed size for a datagram packet */
            static constexpr size_t datagramBufferSize = 4096; /*!< The maximum amount of datagram that can fit the the datagram queue */
            using socket = boost::asio::ip::udp::socket;
            using endpoint = boost::asio::ip::udp::endpoint;

        private:
            /*!
             * @brief Function that launch an async operation that receive a datagram
             */
            void startReceiving();
            /*!
             * @brief Handler called by boost asio when some data was sent
             * @param message A shared pointer containing the datagram that was sent (when the callback return the datagram is destroyed)
             * @param error Boost error that indicate the return of this send action
             * @param byteTransferred The number of bytes transferred
             *
             * WARNING: this function will be called from the network thread NOT the main thread
             */
            void sendHandler(boost::shared_ptr<Datagram> message, const boost::system::error_code &error, size_t byteTransferred);
            /*!
             * @brief Handler called by boost asio when some data is received
             * @param array The data received (will be destroyed when the callback return)
             * @param endpoint The endpoint from which the data was sent
             * @param error Boost error that indicate the return of this receive action
             * @param byteReceived The amount of byte contained in array
             *
             * WARNING: this function will be called from the network thread NOT the main thread
             */
            void receiveHandler(boost::shared_ptr<boost::array<char, maxReceiveDatagramSize>> array, boost::shared_ptr<AsioUdpSocket::endpoint> endpoint, const boost::system::error_code &error, size_t byteReceived);

        private:

            boost::asio::io_context &_context; /*!< The context of this socket */
            socket _socket; /*!< The socket connected to the network */
            boost::lockfree::spsc_queue<Datagram> _datagrams; /*!< The datagram received from the network */
            HostInfos _defaultHost; /*!< The default host to which send the datagrams by default */
        };
    }
}
#endif //BABEL_ASIOUDPSOCKET_HPP
