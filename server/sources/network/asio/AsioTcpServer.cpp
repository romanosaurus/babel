/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** AsioTcpServer.cpp
*/

/* Created the 26/09/2019 at 11:37 by julian.frabel@epitech.eu */

#include <boost/bind.hpp>
#include "network/NetworkException.hpp"
#include "logger/DefaultLogger.hpp"
#include "AsioTcpServer.hpp"

b12software::network::AsioTcpServer::AsioTcpServer(boost::asio::io_context &context)
    : _context(context), _acceptor(_context), _clients(), _clientMapMutex(), _idVectorMutex(), _newIds(), _freeIds(), _disconnectedIds(), _maxId(0)
{

}

b12software::network::AsioTcpServer::~AsioTcpServer()
{

}

void b12software::network::AsioTcpServer::bind(int port)
{
    if (!_acceptor.is_open()) {
        try {
            _acceptor.open(boost::asio::ip::tcp::v4());
        } catch (boost::system::system_error &e) {
            babel::DefaultLogger::Log(babel::LogLevelError, std::string("[AsioTcpServer] Failed to open socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]");
            throw NetworkException(std::string("Failed to open socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]", WHERE);
        }
    }
    try {
        _acceptor.bind(endpoint(boost::asio::ip::tcp::v4(), port));
    } catch (boost::system::system_error &e) {
        babel::DefaultLogger::Log(babel::LogLevelError, std::string("[AsioTcpServer] Failed to bind: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]");
        throw NetworkException(std::string("Failed to bind") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]", WHERE);
    }
    try {
        _acceptor.listen();
    } catch (boost::system::system_error &e) {
        babel::DefaultLogger::Log(babel::LogLevelError, std::string("[AsioTcpServer] Failed to listen: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]");
        throw NetworkException(std::string("Failed to listen") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]", WHERE);
    }
    startAccept();
}

void b12software::network::AsioTcpServer::unbind()
{
    try {
        _acceptor.close();
    } catch (boost::system::system_error &e) {
        babel::DefaultLogger::Log(babel::LogLevelError, std::string("[AsioTcpServer] Failed to close socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]");
        throw NetworkException(std::string("Failed to close socket: ") + e.what() + " [" + std::to_string(e.code().value()) + "(" + e.code().message() + ")]", WHERE);
    }
}

void b12software::network::AsioTcpServer::disconnect(int clientId)
{
    lock lkI(_idVectorMutex);
    lock lkC(_clientMapMutex);
    auto it = _clients.find(clientId);
    if (it != _clients.end()) {
        it->second->disconnect();
        _disconnectedIds.push_back(it->first);
        auto newIt = std::find(_newIds.begin(), _newIds.end(), it->first);
        if (newIt != _newIds.end())
            _newIds.erase(newIt);
        _clients.erase(it);
    }
}

void b12software::network::AsioTcpServer::sendToAll(const void *data, size_t size)
{
    lock lkC(_clientMapMutex);
    for (auto &pair : _clients) {
        pair.second->send(data, size);
    }
}

void b12software::network::AsioTcpServer::sendToAll(const b12software::network::IPacket &packet)
{
    lock lkC(_clientMapMutex);
    for (auto &pair : _clients) {
        pair.second->send(packet);
    }
}

void b12software::network::AsioTcpServer::sendToClient(int clientId, const void *data, size_t size)
{
    lock lkC(_clientMapMutex);
    auto it = _clients.find(clientId);
    if (it != _clients.end()) {
        it->second->send(data, size);
    }
}

void b12software::network::AsioTcpServer::sendToClient(int clientId, const b12software::network::IPacket &packet)
{
    lock lkC(_clientMapMutex);
    auto it = _clients.find(clientId);
    if (it != _clients.end()) {
        it->second->send(packet);
    }
}

void b12software::network::AsioTcpServer::sendToClients(const std::vector<int> &clientIds, const void *data, size_t size)
{
    lock lkC(_clientMapMutex);
    for (auto &id : clientIds) {
        auto it = _clients.find(id);
        if (it != _clients.end()) {
            it->second->send(data, size);
        }
    }
}

void b12software::network::AsioTcpServer::sendToClients(const std::vector<int> &clientIds,
                                                        const b12software::network::IPacket &packet)
{
    lock lkC(_clientMapMutex);
    for (auto &id : clientIds) {
        auto it = _clients.find(id);
        if (it != _clients.end()) {
            it->second->send(packet);
        }
    }
}

void b12software::network::AsioTcpServer::sendExceptClients(const std::vector<int> &exceptIds, const void *data, size_t size)
{
    lock lkC(_clientMapMutex);
    for (auto &pair : _clients) {
        if (std::find(exceptIds.begin(), exceptIds.end(), pair.first) == exceptIds.end())
            pair.second->send(data, size);
    }
}

void b12software::network::AsioTcpServer::sendExceptClients(const std::vector<int> &exceptIds,
                                                            const b12software::network::IPacket &packet)
{
    lock lkC(_clientMapMutex);
    for (auto &pair : _clients) {
        if (std::find(exceptIds.begin(), exceptIds.end(), pair.first) == exceptIds.end())
            pair.second->send(packet);
    }
}

std::vector<int> b12software::network::AsioTcpServer::getConnectedClientIds()
{
    lock lk(_clientMapMutex);
    std::vector<int> ret;
    for (auto &_client : _clients) {
        ret.push_back(_client.first);
    }
    return ret;
}

std::vector<int> b12software::network::AsioTcpServer::getNewConnectedClientIds()
{
    lock lk(_idVectorMutex);
    std::vector<int> ret(_newIds);
    _newIds.clear();
    return ret;
}

std::vector<int> b12software::network::AsioTcpServer::getDisconnectedClientIds()
{
    recomputeDisconnected();
    lock lkI(_idVectorMutex);
    std::vector<int> ret(_disconnectedIds);
    for (auto &id : _disconnectedIds) {
        _freeIds.push(id);
    }
    _disconnectedIds.clear();
    return ret;
}

b12software::network::ITcpClient *b12software::network::AsioTcpServer::getClientById(int clientId)
{
    lock lk(_clientMapMutex);
    auto it = _clients.find(clientId);
    if (it != _clients.end()) {
        return it->second.get();
    }
    return nullptr;
}

void b12software::network::AsioTcpServer::startAccept()
{
    boost::shared_ptr<AsioTcpClient> client(new AsioTcpClient(_context));
    _acceptor.async_accept(
        client->getSocket(),
        boost::bind(&AsioTcpServer::handleAccept, this, client, boost::asio::placeholders::error)
    );
    babel::DefaultLogger::Log(babel::LogLevelDebug, "[AsioTcpServer] Start accepting");
}

void b12software::network::AsioTcpServer::handleAccept(boost::shared_ptr<AsioTcpClient> client,
                                                       const boost::system::error_code &error)
{
    if (error == boost::asio::error::operation_aborted) {
        babel::DefaultLogger::Log(babel::LogLevelInfo, "[AsioTcpServer] Operation aborted (this is due to an async operation still beeing queued while socket is closing.");
        return;
    }
    if (!error) {
        lock lkI(_idVectorMutex);
        lock lkC(_clientMapMutex);
        int id = _maxId + 1;
        if (!_freeIds.empty()) {
            id = _freeIds.front();
            _freeIds.pop();
        } else {
            _maxId++;
        }
        _newIds.push_back(id);
        client->recompute();
        _clients.emplace(id, client);
        babel::DefaultLogger::Log(babel::LogLevelDebug, "[AsioTcpServer] Accepted ID: " + std::to_string(id));
        startAccept();
    } else {
        babel::DefaultLogger::Log(babel::LogLevelError, "[AsioTcpServer] Failed to accept: " + error.message());
    }
}

void b12software::network::AsioTcpServer::recomputeDisconnected()
{
    lock lkI(_idVectorMutex);
    lock lkC(_clientMapMutex);
    for (auto it = _clients.begin(); it != _clients.end(); ) {
        auto curr = it;
        it++;
        if (!curr->second->isConnected()) {
            _disconnectedIds.push_back(curr->first);
            auto newIt = std::find(_newIds.begin(), _newIds.end(), curr->first);
            if (newIt != _newIds.end())
                _newIds.erase(newIt);
            _clients.erase(curr);
        }
    }
}
