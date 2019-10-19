//
// Created by Brian on 29/09/2019.
//

#ifndef BABEL_QTUDPSOCKET_HPP
#define BABEL_QTUDPSOCKET_HPP

#include <QtCore/qobject.h>
#include <QtNetwork/qudpsocket.h>
#include <queue>
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
         * @class QtTcpClient
         * @brief QtNetwork implementation of a tcp client
         */
        class QTUdpSocket : public IUdpSocket, public QObject {
        private:
            using byte = unsigned char;
            static constexpr size_t bufferSizeInBytes = 4096;

        public:
            ~QTUdpSocket() override;
            QTUdpSocket();
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
            /*!
             * @brief ready to read QT data
             */
            void readyToRead();

        private:
            QUdpSocket *_socket;
            HostInfos _defaultHost;
            std::queue<Datagram> _datagrams;
        };
    }
}

#endif //BABEL_QTUDPSOCKET_HPP
