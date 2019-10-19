//
// Created by Brian on 28/09/2019.
//

#ifndef BABEL_QTTCPCLIENT_HPP
#define BABEL_QTTCPCLIENT_HPP

#include <QtNetwork/qtcpsocket.h>
#include "b12software/buffer/CircularBuffer.hpp"
#include "network/ITcpClient.hpp"

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
        class QtTcpClient : public ITcpClient, public QObject {
        private:
            using byte = unsigned char;
            static constexpr size_t bufferSizeInBytes = 4096;

        public:
            ~QtTcpClient() override;
            QtTcpClient();
            void connect(const HostInfos &infos) override;
            void disconnect() override;
            bool isConnected() const override;
            const HostInfos &getHostInfos() const override;
            const HostInfos &getSelfInfos() const override;
            size_t send(const void *data, size_t size) override;
            void send(const IPacket &packet) override;
            size_t receive(void *data, size_t size) override;
            void receive(IPacket &packet) override;
            size_t getAvailableBytesSize() const override;

        private:
            /*!
             * @brief ready to read QT data
             */
            void readyToRead();

            /*!
             * @brief QT client connected to server
             */
            void connected();

        private slots:
            void socketError(QAbstractSocket::SocketError error);

        private:
            QTcpSocket *_socket;
            HostInfos _hostInfo;
            HostInfos _localInfo;
            buffer::CircularBuffer _buffer;
        };
    }
};


#endif //BABEL_QTTCPCLIENT_HPP
