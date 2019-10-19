/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** AsioUdpSocket.cpp
*/

/* Created the 26/09/2019 at 11:42 by julian.frabel@epitech.eu */

#include <boost/bind.hpp>
#include "logger/DefaultLogger.hpp"
#include "AsioUdpSocket.hpp"
#include "network/NetworkException.hpp"

b12software::network::AsioUdpSocket::AsioUdpSocket(boost::asio::io_context &context)
    : _context(context), _socket(context), _datagrams(datagramBufferSize), _defaultHost({"", 0})
{

}

b12software::network::AsioUdpSocket::~AsioUdpSocket()
{

}

void b12software::network::AsioUdpSocket::bind(int port)
{
    if (!_socket.is_open()) {
        try {
            _socket.open(boost::asio::ip::udp::v4());
        } catch (boost::system::system_error &e) {
            babel::DefaultLogger::Log(babel::LogLevelError, std::string("[AsioUdpSocket] Failed to open socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]");
            throw NetworkException(std::string("Failed to open socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]", WHERE);
        }
    }
    try {
        _socket.bind(endpoint(boost::asio::ip::udp::v4(), port));
    } catch (boost::system::system_error &e) {
        babel::DefaultLogger::Log(babel::LogLevelError, std::string("[AsioUdpSocket] Failed to bind: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]");
        throw NetworkException(std::string("Failed to bind: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]", WHERE);
    }
    startReceiving();
}

void b12software::network::AsioUdpSocket::unbind()
{
    try {
        _socket.cancel();
        _socket.close();
    } catch (boost::system::system_error &e) {
        babel::DefaultLogger::Log(babel::LogLevelError, std::string("[AsioUdpSocket] Failed to close socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]");
        throw NetworkException(std::string("Failed to close socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]", WHERE);
    }
}

void b12software::network::AsioUdpSocket::registerHost(const b12software::network::HostInfos &infos)
{
    _defaultHost = infos;
}

void b12software::network::AsioUdpSocket::unregisterHost()
{
    _defaultHost = {"", 0};
}

void b12software::network::AsioUdpSocket::send(const void *data, size_t size, const b12software::network::HostInfos &infos)
{
    send(Datagram(infos, data, size));
}

void b12software::network::AsioUdpSocket::send(const b12software::network::Datagram &datagram)
{
    b12software::network::HostInfos toUse = datagram.getHostInfos();
    if (!_defaultHost.host.empty()) {
        toUse = _defaultHost;
    }

    boost::shared_ptr<Datagram> message(new Datagram(datagram));

    boost::asio::ip::udp::resolver resolver(_context);
    endpoint sendTo = resolver.resolve(toUse.host, std::to_string(toUse.port))->endpoint();
    _socket.async_send_to(
        boost::asio::buffer(message->getData(), message->getDatagramSize()),
        sendTo,
        boost::bind(&AsioUdpSocket::sendHandler, this, message, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );
}

b12software::network::Datagram b12software::network::AsioUdpSocket::receive()
{
    Datagram ret;
    if (hasPendingDatagrams()) {
        ret = _datagrams.front();
        _datagrams.pop();
    }
    return ret;
}

bool b12software::network::AsioUdpSocket::hasPendingDatagrams() const
{
    return _datagrams.read_available() != 0;
}

int b12software::network::AsioUdpSocket::getPort() const
{
    return _socket.local_endpoint().port();
}

std::string b12software::network::AsioUdpSocket::getAddress() const
{
    return _socket.local_endpoint().address().to_string();
}

void b12software::network::AsioUdpSocket::startReceiving()
{
    boost::shared_ptr<boost::array<char, maxReceiveDatagramSize>> buffer(new boost::array<char, maxReceiveDatagramSize>());
    boost::shared_ptr<AsioUdpSocket::endpoint> endpoint(new AsioUdpSocket::endpoint());
    _socket.async_receive_from(
        boost::asio::buffer(*buffer),
        *endpoint,
        boost::bind(&AsioUdpSocket::receiveHandler, this, buffer, endpoint, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );
}

void b12software::network::AsioUdpSocket::sendHandler([[maybe_unused]]boost::shared_ptr<Datagram> message,
                                                      const boost::system::error_code &error,
                                                      [[maybe_unused]]size_t byteTransferred)
{
    if (error == boost::asio::error::operation_aborted) {
        babel::DefaultLogger::Log(babel::LogLevelInfo, "[AsioUdpSocket] Operation aborted (this is due to an async operation still beeing queued while socket is closing.");
        return;
    }
    if (!error) {

    } else {
        babel::DefaultLogger::Log(babel::LogLevelError, "[AsioUdpSocket] Failed to deliver message: " + error.message());
    }
}

void b12software::network::AsioUdpSocket::receiveHandler(boost::shared_ptr<boost::array<char, maxReceiveDatagramSize>> array,
                                                         boost::shared_ptr<AsioUdpSocket::endpoint> endpoint,
                                                         const boost::system::error_code &error, size_t byteReceived)
{
    if (error == boost::asio::error::operation_aborted) {
        babel::DefaultLogger::Log(babel::LogLevelInfo, "[AsioUdpSocket] Operation aborted (this is due to an async operation still beeing queued while socket is closing.");
        return;
    }
    if (!error) {
        HostInfos host = {endpoint->address().to_string(), endpoint->port()};
        if (!_datagrams.push(Datagram(host, array->c_array(), byteReceived))) {
            babel::DefaultLogger::Log(babel::LogLevelWarn, "[AsioUdpSocket] Lost a datagram due to datagram buffer beeing full, consume datagrams faster or augment buffer size");
        }
        startReceiving();
    } else {
        babel::DefaultLogger::Log(babel::LogLevelError, "[AsioUdpSocket] Error when receiving a datagram: " + error.message());
    }
}
