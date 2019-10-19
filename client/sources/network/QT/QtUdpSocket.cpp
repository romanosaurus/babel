//
// Created by Brian on 29/09/2019.
//

#include <QtNetwork/qabstractsocket.h>
#include <iostream>
#include "logger/DefaultLogger.hpp"
#include "network/NetworkException.hpp"
#include "QtUdpSocket.hpp"
#include "QtTcpClient.hpp"

b12software::network::QTUdpSocket::~QTUdpSocket()
{
    if (_socket)
        delete _socket;
}

b12software::network::QTUdpSocket::QTUdpSocket()
    :   _socket(new QUdpSocket(this)),
        _defaultHost({"", 0}),
        _datagrams()
{
}

void b12software::network::QTUdpSocket::bind(int port)
{
    _socket->abort();
    if (!_socket->bind(QHostAddress::AnyIPv4, port))
        throw NetworkException("Failed to bind udp socket", WHERE);
}

void b12software::network::QTUdpSocket::unbind()
{
    _socket->close();
}

void b12software::network::QTUdpSocket::registerHost(const b12software::network::HostInfos &infos)
{
    _defaultHost = infos;
}

void b12software::network::QTUdpSocket::unregisterHost()
{
    _defaultHost = {"", 0};
}

void
b12software::network::QTUdpSocket::send(const void *data, size_t size, const b12software::network::HostInfos &infos)
{
    send(Datagram(infos, data, size));
}

void b12software::network::QTUdpSocket::send(const b12software::network::Datagram &datagram)
{
    HostInfos infos = datagram.getHostInfos();
    if (datagram.getHostInfos().host == "")
        infos = _defaultHost;
    int err = _socket->writeDatagram((const char *)datagram.getData(), datagram.getDatagramSize(), QHostAddress(QString(infos.host.c_str())), infos.port);
    if (err == -1)
        babel::DefaultLogger::Log(babel::LogLevelInfo, "[QTUDP] Erreur when try to write a datagram: " + std::string(_socket->errorString().toUtf8().constData()));

}

b12software::network::Datagram b12software::network::QTUdpSocket::receive()
{
    QHostAddress host;
    quint16 port;
    char buffer[bufferSizeInBytes];
    if (_socket->hasPendingDatagrams()) {
        int size = _socket->readDatagram(buffer, _socket->bytesAvailable(), &host, &port);
        if (size == -1) {
            babel::DefaultLogger::Log(babel::LogLevelWarn, "[QT] Failed to read from server");
            return Datagram();
        }
        return Datagram({host.toString().toUtf8().constData(), port}, buffer, size);
    }
    return Datagram();
}

bool b12software::network::QTUdpSocket::hasPendingDatagrams() const
{
    return _socket->hasPendingDatagrams();
}

int b12software::network::QTUdpSocket::getPort() const
{
    return _socket->localPort();
}

std::string b12software::network::QTUdpSocket::getAddress() const
{
    return _socket->localAddress().toString().toUtf8().constData();

}
