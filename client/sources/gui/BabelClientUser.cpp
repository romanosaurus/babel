//
// Created by Romain FOUYER on 2019-09-27.
//

#include <iostream>
#include "BabelClientUser.hpp"

b12software::babel::BabelClientUser::BabelClientUser(const std::string &userName, const std::string &avatarPath, const std::string &emailAddress, long id)
: _userName(userName),
  _avatarPath(avatarPath),
  _emailAddress(emailAddress),
  _id(id),
  _inCall(false)
{
}

const std::string &b12software::babel::BabelClientUser::getUsername() const
{
    return _userName;
}

const std::string &b12software::babel::BabelClientUser::getAvatarPath() const
{
    return _avatarPath;
}

const std::string& b12software::babel::BabelClientUser::getEmailAddress() const
{
    return _emailAddress;
}

long b12software::babel::BabelClientUser::getId() const
{
    return _id;
}

void b12software::babel::BabelClientUser::setUsername(const std::string &newUsername)
{
    _userName = newUsername;
}

void b12software::babel::BabelClientUser::setAvatarPath(const std::string &newAvatarPath)
{
    _avatarPath = newAvatarPath;
}

void b12software::babel::BabelClientUser::setEmailAddress(const std::string &newEmailAddress)
{
    _emailAddress = newEmailAddress;
}

void b12software::babel::BabelClientUser::setId(long id)
{
    _id = id;
}

void b12software::babel::BabelClientUser::setInCall(bool newState)
{
    _inCall = newState;
}

bool b12software::babel::BabelClientUser::isInCall() const
{
    return _inCall;
}