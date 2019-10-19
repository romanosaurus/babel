//
// Created by Brian on 03/10/2019.
//

#include <regex>
#include <thread>
#include <csignal>
#include <chrono>
#include "logger/DefaultLogger.hpp"
#include "b12software/db/orm/B12Orm.hpp"
#include "exception/db/SqliteException.hpp"
#include "network/asio/AsioTcpServer.hpp"
#include "BabelServer.hpp"

bool b12software::babel::BabelServer::caughtSigInt = false;

const std::map<uint16_t, b12software::babel::BabelServer::handleFunc> b12software::babel::BabelServer::opCodeHandleMap = {
    {b12software::babel::BabelPacket::BPOpCodeAddUserContactList, &BabelServer::handleAddFriendRequest}, // 200
    {b12software::babel::BabelPacket::BPOpCodeDeleteUserContactList, &BabelServer::handleDeleteFriendRequest}, //201
    {b12software::babel::BabelPacket::BPOpCodeListContactList, &BabelServer::handleFriendListRequest }, //202
    {b12software::babel::BabelPacket::BPOpCodeSendResponseContactRequest, &BabelServer::handleFriendRequestResponse}, //203
    {b12software::babel::BabelPacket::BPOpCodeListContactInfo, &BabelServer::handleProfileRequest}, //204
    {b12software::babel::BabelPacket::BPOpCodeListContactRequestList, &BabelServer::handleListFriendsRequest}, // 205
    {b12software::babel::BabelPacket::BPOpCodeStartCall, &BabelServer::handleStartCallRequest}, //210
    {b12software::babel::BabelPacket::BPOpCodeHangupCall, &BabelServer::handleHangUp}, //211
    {b12software::babel::BabelPacket::BPOpCodeSendResponseCall, &BabelServer::handleCallResponse}, //212
    {b12software::babel::BabelPacket::BPOpCodeSendMessage, &BabelServer::handleSendMessages},
    {b12software::babel::BabelPacket::BPOpCodeRetrieveMessageList, &BabelServer::handleRetrieveMessageList},
    {b12software::babel::BabelPacket::BPOpCodeRegisterNewUser, &BabelServer::handleSignupRequest}, //240
    {b12software::babel::BabelPacket::BPOpCodeLoginUser, &BabelServer::handleSigninRequest}, //241
    //242 => TODO (user) (low priority) BPOpCodeUpdateUser
};

void b12software::babel::BabelServer::SignalHandler([[maybe_unused]] int signal)
{
    caughtSigInt = true;
    b12software::babel::DefaultLogger::Log(LogLevelInfo, "[BabelServer] SigInt caught.");
}

b12software::babel::BabelServer::BabelServer()
    :   _networkManager(new network::AsioNetworkManager()),
        _server(),
        _isRunning(false),
        _clientUserMap()
{
    signal(SIGINT, &SignalHandler);

    _networkManager->start();
    _server = _networkManager->createNewTcpServer();
}

void b12software::babel::BabelServer::run(int port)
{
    _server->bind(port);
    _isRunning = true;
    while (_isRunning) {
        handleConnection();
        handleDisconnection();
        receive();
        if (caughtSigInt)
            stop();
        std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    }
}

void b12software::babel::BabelServer::stop()
{
    _isRunning = false;
    _server->unbind();
    _networkManager->terminate();
}

int b12software::babel::BabelServer::getClientIdByUser(int userId)
{
    for (auto clientUserPair : _clientUserMap) {
        if (clientUserPair.second.getId() == userId)
            return clientUserPair.first;
    }
    return 0;
}

void b12software::babel::BabelServer::receive()
{
    BabelPacket packet;
    for (auto &clientId : _server->getConnectedClientIds()) {
        b12software::network::ITcpClient *client = _server->getClientById(clientId);
        packet.clear();
        if (client != nullptr)
            client->receive(packet);
        if (packet.isValid())
            handlePacket(clientId, packet);
    }
}

void b12software::babel::BabelServer::handleConnection()
{
    for (auto &clientId : _server->getNewConnectedClientIds()) {
        b12software::network::ITcpClient *client = _server->getClientById(clientId);

        if (client != nullptr) {
            b12software::babel::DefaultLogger::Log(
                    LogLevelDebug,
                    "[BabelServer] New Client Connected: " + client->getHostInfos().host + ":" + std::to_string(client->getHostInfos().port) + " with id " + std::to_string(clientId)
            );
            _clientUserMap.emplace(clientId, BabelUser());
        }
    }
}

void b12software::babel::BabelServer::handleDisconnection()
{
    for (auto &clientId : _server->getDisconnectedClientIds()) {
        b12software::babel::DefaultLogger::Log(
            LogLevelDebug,
            "[BabelServer] Client #" + std::to_string(clientId) + " disconnected"
        );
        auto it = _clientUserMap.find(clientId);
        if (it != _clientUserMap.end()) {
            if (it->second.isInCall() || it->second.isCalling()) {
                BabelPacket packet;
                packet.createHangupCallPacket(clientId);
                handleHangUp(clientId, packet);
            }
            _clientUserMap.erase(it);
        }
    }
}

void b12software::babel::BabelServer::handlePacket(int clientId, b12software::babel::BabelPacket &packet)
{
    BabelUser &user = _clientUserMap.find(clientId)->second;
    b12software::babel::DefaultLogger::Log(
        LogLevelDebug,
        "[BabelServer] Got packet from client #" + std::to_string(clientId) + " logged as user " + std::to_string(user.getId()) + " packet op " + std::to_string(packet.getPacketOpCode())
    );
    uint16_t opCode = packet.getPacketOpCode();
    auto it = opCodeHandleMap.find(opCode);
    if (it != opCodeHandleMap.end()) {
        try {
            (*this.*(it->second))(clientId, packet);
        } catch (babel::SqliteException &e) {
            DefaultLogger::Log(LogLevelError, "Invalid sql request: " + std::string(e.what()));
        }
    }
}

void b12software::babel::BabelServer::handleAddFriendRequest(int clientId, b12software::babel::BabelPacket &packet)
{
    bool res;
    BabelUser &user = _clientUserMap.find(clientId)->second;

    if (!user.isLoggedIn()) {
        BabelPacket response;

        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    long newFriendId;
    packet.extractAddUserContactListPacket(newFriendId);
    res = user.addFriend(newFriendId);

    if (!res) {
        BabelPacket response;

        response.createSendBinaryResponsePacket(false, "Error when try to send a friend request to this user.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    } else {
        BabelPacket response;

        response.createSendBinaryResponsePacket(true, "Friend request sended successfully.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
    }

    BabelPacket requestPacket;
    requestPacket.createRequestContactRequestPacket(user.getId());
    int toClientID = getClientIdByUser(newFriendId);

    if (requestPacket.isValid() && toClientID != 0)
        _server->sendToClient(toClientID, requestPacket);
}

void b12software::babel::BabelServer::handleDeleteFriendRequest(int clientId, b12software::babel::BabelPacket &packet)
{
    bool res;
    BabelUser &user = _clientUserMap.find(clientId)->second;

    if (!user.isLoggedIn()) {
        BabelPacket response;

        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    long friendId;
    packet.extractDeleteUserContactListPacket(friendId);
    res = user.deleteFriend(friendId);

    if (!res) {
        BabelPacket response;

        response.createSendBinaryResponsePacket(false, "Error when try to send a friend request to this user.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    } else {
        BabelPacket response;

        response.createSendBinaryResponsePacket(true, "Friend deleted successfully.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
    }

    BabelPacket requestPacket;
    requestPacket.createDeletedUserContactListResponsePacket(user.getId());
    int toClientID = getClientIdByUser(friendId);

    if (requestPacket.isValid() && toClientID != 0)
        _server->sendToClient(toClientID, requestPacket);
}

void b12software::babel::BabelServer::handleFriendListRequest(int clientId, [[maybe_unused]] b12software::babel::BabelPacket &packet)
{
    BabelUser &user = _clientUserMap.find(clientId)->second;
    if (!user.isLoggedIn()) {
        BabelPacket response;

        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }

    std::vector<long> friends;
    bool res = user.listFriends(friends);
    if (!res) {
        BabelPacket response;

        response.createSendBinaryResponsePacket(false, "Error when try to get user friends list");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }

    BabelPacket listPacket;
    listPacket.createListContactListResponsePacket(friends);
    if (listPacket.isValid())
        _server->sendToClient(clientId, listPacket);
}

void b12software::babel::BabelServer::handleFriendRequestResponse(int clientId, b12software::babel::BabelPacket &packet)
{
    BabelPacket response;
    BabelUser &user = _clientUserMap.find(clientId)->second;
    if (!user.isLoggedIn()) {
        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }

    long friendId;
    bool res;
    packet.extractSendResponseContactRequestPacket(friendId, res);

    bool success = user.responseFriend(friendId, res);
    if (success) {
        response.createSendBinaryResponsePacket(true, "Response received.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        response.createRequestUserContactResponsePacket(clientId, res);
        int friendClientId = getClientIdByUser(friendId);

        if (response.isValid())
            _server->sendToClient(friendClientId, response);
    } else {
        response.createSendBinaryResponsePacket(false, "Friend request not found.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
    }
}

void b12software::babel::BabelServer::handleListFriendsRequest(int clientId, [[maybe_unused]]b12software::babel::BabelPacket &packet)
{
    BabelUser &user = _clientUserMap.find(clientId)->second;
    if (!user.isLoggedIn()) {
        BabelPacket response;

        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }

    std::vector<long> friendsRequests;
    bool res = user.listFriendsRequests(friendsRequests);
    if (!res) {
        BabelPacket response;

        response.createSendBinaryResponsePacket(false, "Error when try to get user friends requests list");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }

    BabelPacket listPacket;
    listPacket.createListContactRequestListResponsePacket(friendsRequests);
    if (listPacket.isValid())
        _server->sendToClient(clientId, listPacket);
}

void b12software::babel::BabelServer::handleProfileRequest(int clientId, b12software::babel::BabelPacket &packet)
{
    BabelUser &user = _clientUserMap.find(clientId)->second;
    if (!user.isLoggedIn()) {
        BabelPacket response;

        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        else
            babel::DefaultLogger::Log(babel::LogLevelInfo, "Invalid packet");
        return;
    }
    long userToFind;
    packet.extractListContactInfoPacket(userToFind);

    ORM::Entity entity;
    bool res = user.getProfile(userToFind, entity);

    if (!res) {
        BabelPacket response;

        response.createSendBinaryResponsePacket(false, "You're probably not allowed to do that.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        else
            babel::DefaultLogger::Log(babel::LogLevelInfo, "Invalid packet");
        return;
    }
    BabelPacket response;
    response.createListContactInfoResponsePacket(userToFind, entity["username"], std::stol(entity["date_born"]), entity["email"]);
    if (response.isValid())
        _server->sendToClient(clientId, response);
    else
        babel::DefaultLogger::Log(babel::LogLevelInfo, "Invalid packet");
}

void b12software::babel::BabelServer::handleSignupRequest(int clientId, b12software::babel::BabelPacket &packet)
{
    BabelUser &user = _clientUserMap.find(clientId)->second;
    b12software::babel::BabelPacket response;
    b12software::network::ITcpClient *client = _server->getClientById(clientId);
    std::string username;
    std::string email;
    std::string password;
    long birthDate;
    std::regex validUsername("[A-Za-z_][A-Za-z0-9_]{1,9}");
    std::smatch match;
    std::regex validEmail("^((?!\\.)[\\w-_.]*[^.])(@\\w+)(\\.\\w+(\\.\\w+)?[^.\\W])$");


    packet.extractRegisterNewUserPacket(username, birthDate, email, password);
    if (!std::regex_match(username, match, validUsername)) {
        response.createSendBinaryResponsePacket(false, "Invalid username format.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    if (!std::regex_match(email, match, validEmail)) {
        response.createSendBinaryResponsePacket(false, "Invalid email.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    DefaultLogger::Log(LogLevelDebug, "Invalid username: " + username);

    bool result = user.signup(username, email, password, birthDate);
    response.createSendBinaryResponsePacket(result, result ? "OK" : "Username already taken");
    client->send(response);
}

void b12software::babel::BabelServer::handleSigninRequest(int clientId, b12software::babel::BabelPacket &packet)
{
    BabelUser &user = _clientUserMap.find(clientId)->second;
    b12software::network::ITcpClient *client = _server->getClientById(clientId);
    std::string username;
    std::string password;
    packet.extractLoginUserPacket(username, password);
    bool loggedIn = user.attempt(username, password);
    b12software::babel::BabelPacket response;
    if (loggedIn) {
        response.createUserAuthResponsePacket(user.getId());
    } else {
        response.createSendBinaryResponsePacket(false, "Bad credentials");
    }
    client->send(response);
}

void b12software::babel::BabelServer::handleStartCallRequest(int clientId, b12software::babel::BabelPacket &packet)
{
    BabelPacket response;
    BabelUser &user = _clientUserMap.find(clientId)->second;
    if (!user.isLoggedIn()) {
        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        else
            babel::DefaultLogger::Log(babel::LogLevelInfo, "Invalid packet");
        return;
    }

    if (user.isCalling() || user.isInCall()) {
        response.createSendBinaryResponsePacket(false, "You're already in call");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        else
            babel::DefaultLogger::Log(babel::LogLevelInfo, "Invalid packet");
        return;
    }

    long calledId;
    packet.extractStartCallPacket(calledId);

    if (!user.isFriendWith(calledId)) {
        response.createSendBinaryResponsePacket(false, "You are not a contact of this user.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        else
            babel::DefaultLogger::Log(babel::LogLevelInfo, "Invalid packet");
        return;
    }

    long calledClientId = getClientIdByUser(calledId);
    auto it = _clientUserMap.find(calledClientId);
    if (calledClientId == 0 || it == _clientUserMap.end()) {
        response.createSendBinaryResponsePacket(false, "User is not online.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    BabelUser otherUser = it->second;
    if (otherUser.isCalling() || otherUser.isInCall()) {
        response.createSendBinaryResponsePacket(false, "User is busy.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    user.startCalling(calledId);
    response.createSendBinaryResponsePacket(true, "Calling...");
    if (response.isValid())
        _server->sendToClient(clientId, response);
    b12software::network::ITcpClient *client = _server->getClientById(clientId);
    int udpCallerHost;
    inet_pton(AF_INET, client->getHostInfos().host.c_str(), &udpCallerHost);
    response.createReceivedCallPacket(user.getId(), udpCallerHost, b12software::babel::BabelPacket::udpPort);
    if (response.isValid())
        _server->sendToClient(calledClientId, response);
}

void b12software::babel::BabelServer::handleCallResponse(int clientId, b12software::babel::BabelPacket &packet)
{
    BabelPacket response;
    BabelUser &user = _clientUserMap.find(clientId)->second;
    if (!user.isLoggedIn()) {
        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        else
            babel::DefaultLogger::Log(babel::LogLevelInfo, "Invalid packet");
        return;
    }
    long otherId;
    bool answer;
    packet.extractSendResponseCallPacket(otherId, answer);

    int clientOtherId = getClientIdByUser(otherId);
    auto it = _clientUserMap.find(clientOtherId);
    if (clientOtherId == 0 || it == _clientUserMap.end()) {
        response.createSendBinaryResponsePacket(false, "User is not online.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    BabelUser otherUser = it->second;
    if (otherUser.getCalling() != user.getId()) {
        response.createSendBinaryResponsePacket(false, "User is not calling you.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }

    response.createSendBinaryResponsePacket(true, "OK");
    if (response.isValid())
        _server->sendToClient(clientId, response);

    b12software::network::ITcpClient *client = _server->getClientById(clientId);
    int udpCallerHost;
    inet_pton(AF_INET, client->getHostInfos().host.c_str(), &udpCallerHost);

    response.clear();
    response.createCallUserResponsePacket(user.getId(), answer, udpCallerHost, b12software::babel::BabelPacket::udpPort);
    if (answer) {
        user.enterCall(otherId);
    } else {
        otherUser.enterCall(0);
        otherUser.startCalling(0);
    }

    if (response.isValid())
        _server->sendToClient(clientOtherId, response); 
}

void b12software::babel::BabelServer::handleHangUp(int clientId, [[maybe_unused]]b12software::babel::BabelPacket &packet)
{
    BabelPacket response;
    BabelUser &user = _clientUserMap.find(clientId)->second;

    if (!user.isLoggedIn()) {
        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }

    if (user.isCalling() || user.isInCall()) {
        int otherId = (user.isCalling() ? user.getCalling() : user.getCall());
        int otherClientId = getClientIdByUser(otherId);
        auto it = _clientUserMap.find(otherClientId);
        user.startCalling(0);
        user.enterCall(0);
        if (it == _clientUserMap.end())
            return;
        BabelUser &otherUser = it->second;
        response.createUserHangedUpPacket(user.getId());
        if (response.isValid())
            _server->sendToClient(otherClientId, response);
        otherUser.startCalling(0);
        otherUser.enterCall(0);
        response.createSendBinaryResponsePacket(true, "Stopped Calling");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }

    response.createSendBinaryResponsePacket(false, "You are not in a call.");
    if (response.isValid())
        _server->sendToClient(clientId, response);

}

void b12software::babel::BabelServer::handleSendMessages(int clientId, b12software::babel::BabelPacket &packet)
{
    BabelPacket response;
    BabelUser &user = _clientUserMap.find(clientId)->second;
    if (!user.isLoggedIn()) {
        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    long userId;
    std::string message;
    packet.extractSendMessagePacket(userId, message);

    if (!user.sendMessage(userId, message)) {
        response.createSendBinaryResponsePacket(false, "Error when try to send message.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    BabelPacket messageToOther;
    int toClientID = getClientIdByUser(userId);

    messageToOther.createReceivedMessagePacket(user.getId(), message);
    if (messageToOther.isValid() && toClientID != 0)
        _server->sendToClient(toClientID, messageToOther);
    response.createSendBinaryResponsePacket(true, "Message sended successfully.");
    if (response.isValid())
        _server->sendToClient(clientId, response);
}

void b12software::babel::BabelServer::handleRetrieveMessageList(int clientId, b12software::babel::BabelPacket &packet)
{
    BabelPacket response;
    BabelUser &user = _clientUserMap.find(clientId)->second;
    if (!user.isLoggedIn()) {
        response.createSendBinaryResponsePacket(false, "You're not logged in.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    long userId;
    long startTime;
    long endTime;
    std::vector<std::pair<uint64_t, std::string>> messages;

    packet.extractRetrieveMessageListPacket(userId, startTime, endTime);
    if (!user.retrieveMessageList(userId, startTime, endTime, messages)) {
        response.createSendBinaryResponsePacket(false, "Error when try to retrieve messages.");
        if (response.isValid())
            _server->sendToClient(clientId, response);
        return;
    }
    response.createRetrieveMessageListResponsePacket(userId, messages);

    if (response.isValid())
        _server->sendToClient(clientId, response);
}
