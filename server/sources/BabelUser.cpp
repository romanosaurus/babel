/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** BabelClientUser.cpp
*/

/* Created the 03/10/2019 at 07:18 by julian.frabel@epitech.eu */

#include <algorithm>
#include <api/Api.hpp>
#include "BabelUser.hpp"

b12software::babel::BabelUser::BabelUser()
    : _userId(0), _calling(0), _inCallWith(0)
{

}

bool b12software::babel::BabelUser::isLoggedIn() const
{
    return _userId != 0;
}

bool b12software::babel::BabelUser::isCalling() const
{
    return _calling != 0;
}

bool b12software::babel::BabelUser::isInCall() const
{
    return _inCallWith != 0;
}

int b12software::babel::BabelUser::getId() const
{
    return _userId;
}

bool b12software::babel::BabelUser::attempt(const std::string &username, const std::string &password)
{
    bool result;
    int userId;
    b12software::babel::API::Signin(username, password, [&result, &userId](bool success, int id) {
        result = success;
        userId = id;
    });
    if (result) {
        _userId = userId;
    }
    return result;
}

bool b12software::babel::BabelUser::signup(const std::string &username, const std::string &email,
                                           const std::string &password, long birthdate)
{
    bool result;
    b12software::babel::API::Signup(username, password, email, birthdate, [&result](bool success) {
        result = success;
    });
    return result;
}

bool b12software::babel::BabelUser::addFriend(long newFriendId)
{
    bool result;

    if (isFriendWith(newFriendId))
        return false;
    b12software::babel::API::AddFriend(_userId, newFriendId, [&result](bool success) {
        result = success;
    });
    return result;
}

bool b12software::babel::BabelUser::responseFriend(long friendId, bool response)
{
    bool result;

    b12software::babel::API::AcceptOrRefuseFriend(_userId, friendId, response, [&result](bool success) {
        result = success;
    });
    return result;
}

bool b12software::babel::BabelUser::deleteFriend(long friendId)
{
    bool result;

    b12software::babel::API::RemoveFriend(_userId, friendId, [&result](bool success) {
        result = success;
    });
    return result;
}

bool b12software::babel::BabelUser::listFriends(std::vector<long> &ids)
{
    bool res = true;

    b12software::babel::API::ListFriend(_userId, [&ids, &res](bool success, ORM::Entity entity) {
        if (!success)
            res = false;
        ids.push_back(std::stol(entity["id"]));
    });

    return res;
}

bool b12software::babel::BabelUser::listFriendsRequests(std::vector<long> &ids)
{
    bool res = true;

    b12software::babel::API::ListFriendRequests(_userId, [&ids, &res](bool success, ORM::Entity entity) {
        if (!success)
            res = false;
        ids.push_back(std::stol(entity["from_id"]));
    });

    return res;
}

bool b12software::babel::BabelUser::getProfile(long id, ORM::Entity &resEntity)
{
    bool res;

    std::vector<long> friends;
    babel::API::GetProfile(id, [&res, &resEntity](bool success, ORM::Entity entity) {
        res = success;
        resEntity = entity;
    });
    return res;
}

bool b12software::babel::BabelUser::isFriendWith(int id)
{
    std::vector<long> ids;
    if (listFriends(ids)) {
        if (std::find(ids.begin(), ids.end(), id) != ids.end()) {
            return true;
        }
    }
    return false;
}

void b12software::babel::BabelUser::startCalling(int id)
{
    _calling = id;
}

int b12software::babel::BabelUser::getCalling() const
{
    return _calling;
}

void b12software::babel::BabelUser::enterCall(int id)
{
    _inCallWith = id;
}

int b12software::babel::BabelUser::getCall() const
{
    return _inCallWith;
}

bool b12software::babel::BabelUser::sendMessage(long idTo, const std::string &message)
{
    bool result;

    API::AddMessage(_userId, idTo, message, [&result](bool success) {
        result = success;
    });
    return result;
}

bool b12software::babel::BabelUser::retrieveMessageList(long idTo, long timeStart, long timeEnd,
                                                        std::vector<std::pair<uint64_t, std::string>> &messages)
{
    bool result;

    API::GetMessages(_userId, idTo, timeStart, timeEnd, [&result, &messages](bool success, ORM::Entity entity) {
        result = success;
        messages.emplace_back(std::pair<long, std::string>(std::stol(entity["from_id"]), entity["message"]));
    });

    return result;
}