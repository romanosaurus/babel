
/* Created by Romain FOUYER on 03/10/2019 at 17:41. */


#include "UserUtilities.hpp"

b12software::babel::UserUtilities::UserUtilities()
: _calledId(-1), _calledUser("", "", "", 420), _waitingCall(false)
{
}

b12software::babel::UserUtilities &b12software::babel::UserUtilities::getInstance()
{
    static b12software::babel::UserUtilities utils;

    return utils;
}

void b12software::babel::UserUtilities::setCalledId(long newId)
{
    _calledId = newId;
}

long b12software::babel::UserUtilities::getCalledId() const
{
    return _calledId;
}

void b12software::babel::UserUtilities::setCalledUser(b12software::babel::BabelClientUser &user)
{
    _calledUser = user;
}

b12software::babel::BabelClientUser &b12software::babel::UserUtilities::getCalledUser()
{
    return _calledUser;
}

void b12software::babel::UserUtilities::setCommunicationInfo(int ipAddr, int port)
{
    std::get<0>(_communicationInfo) = ipAddr;
    std::get<1>(_communicationInfo) = port;
}

void b12software::babel::UserUtilities::setWaitingCall(bool newStatus)
{
    _waitingCall = newStatus;
}

bool b12software::babel::UserUtilities::getWaitingCall() const
{
    return _waitingCall;
}