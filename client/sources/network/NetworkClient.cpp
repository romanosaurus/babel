/*
** EPITECH PROJECT, 2019
** test
** File description:
** NetworkClient
*/

#include "NetworkClient.hpp"

b12software::network::NetworkClient::NetworkClient()
    :   _networkManager(new network::QtNetworkManager()),
        _client(_networkManager->createNewTcpClient()),
        _udpSocket(_networkManager->createNewUdpSocket())
{

}

b12software::network::NetworkClient::~NetworkClient()
{
    _udpSocket->unbind();
    disconnectClient();
}

void b12software::network::NetworkClient::run()
{
    connectClient();
}

void b12software::network::NetworkClient::connectClient()
{
    namespace pt = boost::property_tree;
    
    if (!boost::filesystem::exists("config.json"))
    {
        pt::ptree createFile;
        createFile.put("port", "42069");
        createFile.put("host", "localhost");
        pt::write_json("config.json", createFile);
    }

    pt::ptree root;
    pt::read_json("config.json", root);

    std::pair<std::string, std::string> result;

    result.first = root.get<std::string>("port");
    result.second = root.get<std::string>("host");

    HostInfos infoClient;
    infoClient.port = std::stoi(result.first);
    infoClient.host = result.second;
    _client->connect(infoClient);
}

void b12software::network::NetworkClient::disconnectClient()
{
    _client->disconnect();
}

std::shared_ptr<b12software::network::ITcpClient> b12software::network::NetworkClient::getClient()
{
    return _client;
}

std::shared_ptr<b12software::network::IUdpSocket> b12software::network::NetworkClient::getUdpSocket()
{
    return _udpSocket;
}