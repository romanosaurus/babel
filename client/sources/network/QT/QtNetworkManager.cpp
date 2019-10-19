//
// Created by Brian on 01/10/2019.
//

#include "network/QT/QtUdpSocket.hpp"
#include "network/QT/QtTcpClient.hpp"
#include "QtNetworkManager.hpp"

b12software::network::QtNetworkManager::QtNetworkManager()
    :   _updSockets(), _tcpClients()
{

}

b12software::network::QtNetworkManager::~QtNetworkManager()
{
    for (auto udp : _updSockets)
        delete udp;
    for (auto tcp : _tcpClients)
        delete tcp;
}

b12software::network::IUdpSocket *b12software::network::QtNetworkManager::createNewUdpSocket()
{
    IUdpSocket *sock = new QTUdpSocket();

    _updSockets.push_back(sock);
    return sock;
}

b12software::network::ITcpClient *b12software::network::QtNetworkManager::createNewTcpClient()
{
    ITcpClient *client = new QtTcpClient();

    _tcpClients.push_back(client);
    return client;
}

b12software::network::ITcpServer *b12software::network::QtNetworkManager::createNewTcpServer()
{
    return nullptr;
}

void b12software::network::QtNetworkManager::destroyUdpSocket(b12software::network::IUdpSocket *socket)
{
    auto it = std::find(_updSockets.begin(), _updSockets.end(), socket);
    if (it != _updSockets.end()) {
        _updSockets.erase(it);
        delete socket;
    }
}

void b12software::network::QtNetworkManager::destroyTcpClient(b12software::network::ITcpClient *client)
{
    auto it = std::find(_tcpClients.begin(), _tcpClients.end(), client);
    if (it != _tcpClients.end()) {
        _tcpClients.erase(it);
        delete client;
    }
}

void b12software::network::QtNetworkManager::destroyTcpServer(b12software::network::ITcpServer *server)
{
    (void)server;
}

void b12software::network::QtNetworkManager::start()
{

}

void b12software::network::QtNetworkManager::terminate()
{

}
