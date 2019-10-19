//
// Created by Brian on 28/09/2019.
//

#include <QtCore/QDataStream>
#include <iostream>
#include "logger/DefaultLogger.hpp"
#include "network/NetworkException.hpp"
#include "QtTcpClient.hpp"

b12software::network::QtTcpClient::~QtTcpClient()
{
    delete _socket;
}

b12software::network::QtTcpClient::QtTcpClient()
    :   _socket(new QTcpSocket(this)),
        _hostInfo({"", 0}),
        _localInfo({"", 0}),
        _buffer(bufferSizeInBytes)
{
}

void b12software::network::QtTcpClient::connect(const b12software::network::HostInfos &infos)
{
    _socket->abort();
    _socket->connectToHost(QString(infos.host.c_str()), infos.port);

    if (!_socket->waitForConnected(1000)) {
        throw NetworkException("failed to connect to server", WHERE);
    }
    QObject::connect(_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &QtTcpClient::socketError);
    QObject::connect(_socket, &QTcpSocket::connected, this, &QtTcpClient::connected);
    QObject::connect(_socket, &QTcpSocket::readyRead, this, &QtTcpClient::readyToRead);
}

void b12software::network::QtTcpClient::disconnect()
{
    if (isConnected())
        _socket->disconnectFromHost();
}

bool b12software::network::QtTcpClient::isConnected() const
{
    return _socket->state() == QAbstractSocket::ConnectedState;
}

const b12software::network::HostInfos &b12software::network::QtTcpClient::getHostInfos() const
{
    return _hostInfo;
}

const b12software::network::HostInfos &b12software::network::QtTcpClient::getSelfInfos() const
{
    return _localInfo;
}

size_t b12software::network::QtTcpClient::send(const void *data, size_t size)
{
    int ret = _socket->write((const char *)data, size);

    if (ret == -1)
        babel::DefaultLogger::Log(babel::LogLevelWarn, "[QT] Failed to write data on server");
    _socket->waitForBytesWritten();
    return ret;
}

void b12software::network::QtTcpClient::send(const b12software::network::IPacket &packet)
{
    if (packet.isValid())
        send(packet.getData(), packet.getSize());
}

size_t b12software::network::QtTcpClient::receive(void *data, size_t size)
{
    return _buffer.read(static_cast<byte *>(data), size);
}

void b12software::network::QtTcpClient::receive(b12software::network::IPacket &packet)
{
    packet.clear();
    byte tmp[bufferSizeInBytes];

    size_t bufferedBytes = _buffer.read(tmp, getAvailableBytesSize());
    size_t retrievedData = packet.setData(tmp, bufferedBytes);
    if (retrievedData != bufferedBytes) {
        size_t diff = bufferedBytes - retrievedData;
        _buffer.write(tmp + retrievedData, diff);
    }
}

size_t b12software::network::QtTcpClient::getAvailableBytesSize() const
{
    return _buffer.getNbBufferedBytes() * sizeof(byte);
}

void b12software::network::QtTcpClient::readyToRead()
{
    char buffer[bufferSizeInBytes];

    int size = _socket->read(buffer, bufferSizeInBytes);
    if (size == -1)
        return babel::DefaultLogger::Log(babel::LogLevelWarn, "[QT] Failed to read from server");

    if (_buffer.write(buffer, size)) {
        return babel::DefaultLogger::Log(babel::LogLevelWarn, "[QT] Internal buffer overflowed, some data was lost");
    }
}

void b12software::network::QtTcpClient::connected()
{
    _hostInfo = {_socket->peerName().toUtf8().constData(), _socket->peerPort()};
    _localInfo = {"", _socket->localPort()};
}

void b12software::network::QtTcpClient::socketError([[maybe_unused]] QAbstractSocket::SocketError error)
{
    throw b12software::network::NetworkException("Error on Socket", WHERE);
}
