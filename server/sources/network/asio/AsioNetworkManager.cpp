/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** AsioNetworkManager.cpp
*/

/* Created the 26/09/2019 at 11:19 by julian.frabel@epitech.eu */

#include "AsioNetworkManager.hpp"
#include "AsioUdpSocket.hpp"
#include "AsioTcpClient.hpp"
#include "AsioTcpServer.hpp"
#include "logger/DefaultLogger.hpp"

b12software::network::AsioNetworkManager::AsioNetworkManager()
    : _ioContext(), _worker(), _udpSockets(), _tcpClients(), _tcpServers(), _networkThread(), _started(false)
{
    babel::DefaultLogger::Log(babel::LogLevelDebug, "[AsioNetworkManager] Created network manager");
}

b12software::network::AsioNetworkManager::~AsioNetworkManager()
{
    terminate();
    babel::DefaultLogger::Log(babel::LogLevelDebug, "[AsioNetworkManager] Destroyed network manager");
}

void b12software::network::AsioNetworkManager::start()
{
    if (_started)
        return;
    _worker = new boost::asio::io_context::work(_ioContext);
    _networkThread = boost::thread(&networkingThreadFunc, this);
    _started = true;
}

void b12software::network::AsioNetworkManager::terminate()
{
    if (!_started)
        return;
    for (auto &ptr : _udpSockets) {
        delete ptr;
    }
    for (auto &ptr : _tcpClients) {
        delete ptr;
    }
    for (auto &ptr : _tcpServers) {
        delete ptr;
    }
    delete _worker;
    _networkThread.join();
    _started = false;
}

void b12software::network::AsioNetworkManager::networkingThreadFunc(AsioNetworkManager *manager)
{
    babel::DefaultLogger::Log(babel::LogLevelDebug, "[AsioNetworkManager] Started network thread");
    manager->_ioContext.run();
    babel::DefaultLogger::Log(babel::LogLevelDebug, "[AsioNetworkManager] Network thread terminating");
}

b12software::network::IUdpSocket *b12software::network::AsioNetworkManager::createNewUdpSocket()
{
    if (!_started)
        return nullptr;
    IUdpSocket *sock = new AsioUdpSocket(_ioContext);
    _udpSockets.push_back(sock);
    return sock;
}

b12software::network::ITcpClient *b12software::network::AsioNetworkManager::createNewTcpClient()
{
    if (!_started)
        return nullptr;
    ITcpClient *client = new AsioTcpClient(_ioContext);
    _tcpClients.push_back(client);
    return client;
}

b12software::network::ITcpServer *b12software::network::AsioNetworkManager::createNewTcpServer()
{
    if (!_started)
        return nullptr;
    ITcpServer *server = new AsioTcpServer(_ioContext);
    _tcpServers.push_back(server);
    return server;
}

void b12software::network::AsioNetworkManager::destroyUdpSocket(b12software::network::IUdpSocket *socket)
{
    auto it = std::find(_udpSockets.begin(), _udpSockets.end(), socket);
    if (it != _udpSockets.end()) {
        _udpSockets.erase(it);
        delete socket;
    }
}

void b12software::network::AsioNetworkManager::destroyTcpClient(b12software::network::ITcpClient *client)
{
    auto it = std::find(_tcpClients.begin(), _tcpClients.end(), client);
    if (it != _tcpClients.end()) {
        _tcpClients.erase(it);
        delete client;
    }
}

void b12software::network::AsioNetworkManager::destroyTcpServer(b12software::network::ITcpServer *server)
{
    auto it = std::find(_tcpServers.begin(), _tcpServers.end(), server);
    if (it != _tcpServers.end()) {
        _tcpServers.erase(it);
        delete server;
    }
}
