/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** AsioTcpClient.cpp
*/

/* Created the 26/09/2019 at 11:27 by julian.frabel@epitech.eu */

#include <boost/bind.hpp>
#include "AsioTcpClient.hpp"
#include "logger/DefaultLogger.hpp"
#include "network/NetworkException.hpp"

b12software::network::AsioTcpClient::AsioTcpClient(boost::asio::io_context &context)
    : _context(context), _socket(context), _distantInfos({"", 0}), _selfInfos({"", 0}), _buffer(bufferSizeInBytes)
{

}

b12software::network::AsioTcpClient::~AsioTcpClient()
{

}

void b12software::network::AsioTcpClient::connect(const b12software::network::HostInfos &infos)
{
    if (!_socket.is_open()) {
        try {
            _socket.open(boost::asio::ip::tcp::v4());
        } catch (boost::system::system_error &e) {
            babel::DefaultLogger::Log(babel::LogLevelError, std::string("[AsioTcpClient] Failed to open socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]");
            throw NetworkException(std::string("Failed to open socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]", WHERE);
        }
    }
    boost::asio::ip::tcp::resolver resolver(_context);
    boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(infos.host, std::to_string(infos.port));
    _distantInfos = {"", 0};
    _selfInfos = {"", 0};
    endpoint endPoint = *it;
    _socket.async_connect(endPoint, boost::bind(&AsioTcpClient::handleConnect, this, boost::asio::placeholders::error, ++it));
}

void b12software::network::AsioTcpClient::disconnect()
{
    _distantInfos = {"", 0};
    _selfInfos = {"", 0};
    try {
        _socket.close();
    } catch (boost::system::system_error &e) {
        babel::DefaultLogger::Log(babel::LogLevelError, std::string("[AsioTcpClient] Failed to close socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]");
        throw NetworkException(std::string("Failed to close socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]", WHERE);
    }
}

bool b12software::network::AsioTcpClient::isConnected() const
{
    return !_distantInfos.host.empty();
}

const b12software::network::HostInfos &b12software::network::AsioTcpClient::getHostInfos() const
{
    return _distantInfos;
}

const b12software::network::HostInfos &b12software::network::AsioTcpClient::getSelfInfos() const
{
    return _selfInfos;
}

size_t b12software::network::AsioTcpClient::send(const void *data, size_t size)
{
    boost::shared_ptr<byte[]> message(new byte[size]);
    std::memcpy(message.get(), data, size);
    _socket.async_send(
        boost::asio::buffer(message.get(), size),
        boost::bind(&AsioTcpClient::handleSend, this, message, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );
    return 0;
}

void b12software::network::AsioTcpClient::send(const b12software::network::IPacket &packet)
{
    if (packet.isValid())
        send(packet.getData(), packet.getSize());
}

size_t b12software::network::AsioTcpClient::receive(void *data, size_t size)
{
    return _buffer.read(static_cast<byte *>(data), size);
}

void b12software::network::AsioTcpClient::receive(b12software::network::IPacket &packet)
{
    packet.clear();
    _buffer.lockBuffer();
    byte tmp[bufferSizeInBytes];
    size_t bufferedBytes = _buffer.read(tmp, bufferSizeInBytes);
    size_t retrievedData = packet.setData(tmp, bufferedBytes);
    if (retrievedData != bufferedBytes) {
        size_t diff = bufferedBytes - retrievedData;
        _buffer.write(tmp + retrievedData, diff);
    }
    _buffer.unlockBuffer();
}

size_t b12software::network::AsioTcpClient::getAvailableBytesSize() const
{
    return _buffer.getNbBufferedElements() * sizeof(byte);
}

void b12software::network::AsioTcpClient::startReceive()
{
    boost::shared_ptr<byte[]> message(new byte[bufferSizeInBytes]);
    _socket.async_receive(
        boost::asio::buffer(message.get(), bufferSizeInBytes),
        boost::bind(&AsioTcpClient::handleReceive, this, message, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );
}

void b12software::network::AsioTcpClient::handleConnect(const boost::system::error_code &error,
                                                        boost::asio::ip::tcp::resolver::iterator endpointIterator)
{
    if (error == boost::asio::error::operation_aborted) {
        babel::DefaultLogger::Log(babel::LogLevelInfo, "[AsioTcpClient] Operation aborted (this is due to an async operation still beeing queued while socket is closing.");
        return;
    }
    if (!error) {
        recompute();
    } else if (endpointIterator != boost::asio::ip::tcp::resolver::iterator()) {
        _socket.close();
        endpoint endPoint = *endpointIterator;
        _socket.async_connect(endPoint, boost::bind(&AsioTcpClient::handleConnect, this, boost::asio::placeholders::error, ++endpointIterator));
    } else {
        babel::DefaultLogger::Log(babel::LogLevelError, "[AsioTcpClient] Failed to connect to server: " + error.message());
    }
}

void b12software::network::AsioTcpClient::handleSend([[maybe_unused]]boost::shared_ptr<byte[]> message,
                                                     const boost::system::error_code &error,
                                                     [[maybe_unused]]size_t byteTransferred)
{
    if (error == boost::asio::error::operation_aborted) {
        babel::DefaultLogger::Log(babel::LogLevelInfo, "[AsioTcpClient] Operation aborted (this is due to an async operation still beeing queued while socket is closing.");
        return;
    }
    if (!error) {

    } else {
        babel::DefaultLogger::Log(babel::LogLevelError, "[AsioTcpClient] Failed to send data: " + error.message());
        disconnect();
    }
}

void b12software::network::AsioTcpClient::handleReceive(boost::shared_ptr<byte[]> message,
                                                        const boost::system::error_code &error,
                                                        size_t byteTransferred)
{
    if (error == boost::asio::error::operation_aborted) {
        babel::DefaultLogger::Log(babel::LogLevelInfo, "[AsioTcpClient] Operation aborted (this is due to an async operation still beeing queued while socket is closing.");
        return;
    }
    if (!error) {
        if (_buffer.write(message.get(), byteTransferred)) {
            babel::DefaultLogger::Log(babel::LogLevelWarn, "[AsioTcpClient] Internal buffer overflowed, some data was lost");
        }
        startReceive();
    } else {
        babel::DefaultLogger::Log(babel::LogLevelError, "[AsioTcpClient] Failed to receive data: " + error.message());
        disconnect();
    }
}

b12software::network::AsioTcpClient::socket &b12software::network::AsioTcpClient::getSocket()
{
    return _socket;
}

void b12software::network::AsioTcpClient::recompute()
{
    _distantInfos = {_socket.remote_endpoint().address().to_string(), _socket.remote_endpoint().port()};
    _selfInfos = {_socket.local_endpoint().address().to_string(), _socket.local_endpoint().port()};
    startReceive();
}
