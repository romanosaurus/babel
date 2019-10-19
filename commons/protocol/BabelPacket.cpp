/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** BabelPacket.cpp
*/

/* Created the 01/10/2019 at 16:02 by julian.frabel@epitech.eu */

#include <iostream>
#include <cstring>
#include "BabelPacket.hpp"

const std::map<uint16_t, b12software::babel::BabelPacket::extractFunc> b12software::babel::BabelPacket::_opCodeExtractFuncMap = {
    {BPOpCodeSendBinaryResponse, &BabelPacket::buildSendBinaryResponseFromData},
    {BPOpCodeAddUserContactList, &BabelPacket::buildAddUserContactListFromData},
    {BPOpCodeDeleteUserContactList, &BabelPacket::buildDeleteUserContactListFromData},
    {BPOpCodeListContactList, &BabelPacket::buildListContactListFromData},
    {BPOpCodeSendResponseContactRequest, &BabelPacket::buildSendResponseContactRequestFromData},
    {BPOpCodeListContactInfo, &BabelPacket::buildListContactInfoFromData},
    {BPOpCodeListContactRequestList, &BabelPacket::buildCreateListContactRequestListPacketFromData},
    {BPOpCodeStartCall, &BabelPacket::buildStartCallFromData},
    {BPOpCodeHangupCall, &BabelPacket::buildHangupCallFromData},
    {BPOpCodeSendResponseCall, &BabelPacket::buildSendResponseCallFromData},
    {BPOpCodeSendMessage, &BabelPacket::buildSendMessageFromData},
    {BPOpCodeRetrieveMessageList, &BabelPacket::buildRetrieveMessageListFromData},
    {BPOpCodeRegisterNewUser, &BabelPacket::buildRegisterNewUserFromData},
    {BPOpCodeLoginUser, &BabelPacket::buildLoginUserFromData},
    {BPOpCodeUpdateUser, &BabelPacket::buildUpdateUserFromData},
    {BPOpCodeRequestContactRequest, &BabelPacket::buildRequestContactRequestFromData},
    {BPOpCodeRequestUserContactResponse, &BabelPacket::buildRequestUserContactResponseFromData},
    {BPOpCodeDeletedUserContactListResponse, &BabelPacket::buildDeletedUserContactListResponseFromData},
    {BPOpCodeListContactListResponse, &BabelPacket::buildListContactListResponseFromData},
    {BPOpCodeListContactInfoResponse, &BabelPacket::buildListContactInfoResponseFromData},
    {BPOpCodeListContactRequestListResponse, &BabelPacket::buildListContactRequestListResponseFromData},
    {BPOpCodeReceivedCall, &BabelPacket::buildReceivedCallFromData},
    {BPOpCodeCallUserResponse, &BabelPacket::buildCallUserResponseFromData},
    {BPOpCodeUserHangedUp, &BabelPacket::buildUserHangedUpFromData},
    {BPOpCodeReceivedMessage, &BabelPacket::buildReceivedMessageFromData},
    {BPOpCodeRetrieveMessageListResponse, &BabelPacket::buildRetrieveMessageListResponseFromData},
    {BPOpCodeUserAuthResponse, &BabelPacket::buildUserAuthResponseFromData},
};

b12software::babel::BabelPacket::BabelPacket()
    : _valid(false), _packetSize(0), _rawData()
{

}

b12software::babel::BabelPacket::~BabelPacket()
{

}

size_t b12software::babel::BabelPacket::getSize() const
{
    return _packetSize;
}

const void *b12software::babel::BabelPacket::getData() const
{
    return _rawData.byteArray;
}

size_t b12software::babel::BabelPacket::setData(const void *data, size_t size)
{
    if (size < 1)
        return 0;
    const uint16_t *opCodeArray = static_cast<const uint16_t *>(data);
    uint16_t opCode = NTOHS(*opCodeArray);
    auto it = _opCodeExtractFuncMap.find(opCode);
    if (it == _opCodeExtractFuncMap.end()) // discard invalid op code to try to slide to a valid op code
        return 1;
    return (*this.*(it->second))(data, size);
}

void b12software::babel::BabelPacket::clear()
{
    _packetSize = 0;
    _valid = false;
    std::memset(_rawData.byteArray, 0, sizeof(_rawData.byteArray));
}

bool b12software::babel::BabelPacket::isValid() const
{
    return _valid;
}

void b12software::babel::BabelPacket::createSendBinaryResponsePacket(bool response, const std::string &description)
{
    _valid = true;
    size_t descriptionSize = (description.size() <= sizeof(_rawData.formatted.data._100.msg)) ? description.size() : sizeof(_rawData.formatted.data._100.msg);
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._100.response) + sizeof(_rawData.formatted.data._100.msgLength) + descriptionSize;
    _rawData.formatted.type = HTONS(BPOpCodeSendBinaryResponse);
    _rawData.formatted.data._100.response = response;
    _rawData.formatted.data._100.msgLength = HTONLL(descriptionSize);
    std::memcpy(_rawData.formatted.data._100.msg, description.c_str(), descriptionSize);
}

void b12software::babel::BabelPacket::createAddUserContactListPacket(long userId)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    _rawData.formatted.type = HTONS(BPOpCodeAddUserContactList);
    _rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId = HTONLL(userId);
}

void b12software::babel::BabelPacket::createDeleteUserContactListPacket(long userId)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    _rawData.formatted.type = HTONS(BPOpCodeDeleteUserContactList);
    _rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId = HTONLL(userId);
}

void b12software::babel::BabelPacket::createListContactListPacket()
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode);
    _rawData.formatted.type = HTONS(BPOpCodeListContactList);
}

void b12software::babel::BabelPacket::createSendResponseContactRequestPacket(long userId, bool response)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._203_212_301);
    _rawData.formatted.type = HTONS(BPOpCodeSendResponseContactRequest);
    _rawData.formatted.data._203_212_301.targetId = HTONLL(userId);
    _rawData.formatted.data._203_212_301.response = response;
}

void b12software::babel::BabelPacket::createListContactInfoPacket(long userId)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    _rawData.formatted.type = HTONS(BPOpCodeListContactInfo);
    _rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId = HTONLL(userId);
}

void b12software::babel::BabelPacket::createListContactRequestListPacket()
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode);
    _rawData.formatted.type = HTONS(BPOpCodeListContactRequestList);
}

void b12software::babel::BabelPacket::createStartCallPacket(long userId)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    _rawData.formatted.type = HTONS(BPOpCodeStartCall);
    _rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId = HTONLL(userId);
}

void b12software::babel::BabelPacket::createHangupCallPacket(long userId)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    _rawData.formatted.type = HTONS(BPOpCodeHangupCall);
    _rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId = HTONLL(userId);
}

void b12software::babel::BabelPacket::createSendResponseCallPacket(long userId, bool response)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._203_212_301);
    _rawData.formatted.type = HTONS(BPOpCodeSendResponseCall);
    _rawData.formatted.data._203_212_301.targetId = HTONLL(userId);
    _rawData.formatted.data._203_212_301.response = response;
}

void b12software::babel::BabelPacket::createSendMessagePacket(long userId, const std::string &message)
{
    _valid = true;
    size_t messageSize = (message.size() <= sizeof(_rawData.formatted.data._220_320.msg)) ? message.size() : sizeof(_rawData.formatted.data._220_320.msg);
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._220_320.targetId) + sizeof(_rawData.formatted.data._220_320.msgLength) + messageSize;
    _rawData.formatted.type = HTONS(BPOpCodeSendMessage);
    _rawData.formatted.data._220_320.targetId = HTONLL(userId);
    _rawData.formatted.data._220_320.msgLength = HTONLL(messageSize);
    std::memcpy(_rawData.formatted.data._220_320.msg, message.c_str(), messageSize);
}

void b12software::babel::BabelPacket::createRetrieveMessageListPacket(long userId, long startTimeStamp, long endTimeStamp)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._221);
    _rawData.formatted.type = HTONS(BPOpCodeRetrieveMessageList);
    _rawData.formatted.data._221.targetId = HTONLL(userId);
    _rawData.formatted.data._221.startTimeStamp = HTONLL(startTimeStamp);
    _rawData.formatted.data._221.endTimeStamp = HTONLL(endTimeStamp);
}

void b12software::babel::BabelPacket::createRegisterNewUserPacket(const std::string &username, long birthDate,
                                                                  const std::string &email, const std::string &password)
{
    size_t passwordLength = password.length();
    size_t emailLength = email.length();
    if (passwordLength + emailLength > sizeof(_rawData.formatted.data._240.emailAndPassword))
        return;
    if (username.length() > 10)
        return;
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._240.username) + sizeof(_rawData.formatted.data._240.birthDate) + sizeof(_rawData.formatted.data._240.emailLength) + sizeof(_rawData.formatted.data._240.passwordLength) + passwordLength + emailLength;
    _rawData.formatted.type = HTONS(BPOpCodeRegisterNewUser);
    std::memset(_rawData.formatted.data._240.username, 0, sizeof(_rawData.formatted.data._240.username));
    std::memcpy(_rawData.formatted.data._240.username, username.c_str(), username.length());
    _rawData.formatted.data._240.birthDate = HTONLL(birthDate);
    _rawData.formatted.data._240.passwordLength = HTONLL(passwordLength);
    _rawData.formatted.data._240.emailLength = HTONLL(emailLength);
    std::memcpy(_rawData.formatted.data._240.emailAndPassword, email.c_str(), emailLength);
    std::memcpy(reinterpret_cast<void *>(reinterpret_cast<intptr_t>(_rawData.formatted.data._240.emailAndPassword) + emailLength), password.c_str(), passwordLength);
}

void b12software::babel::BabelPacket::createLoginUserPacket(const std::string &username, const std::string &password)
{
    size_t passwordLength = password.length();
    if (username.length() > 10)
        return;
    if (passwordLength > sizeof(_rawData.formatted.data._241.password))
        return;
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._241.username) + sizeof(_rawData.formatted.data._241.passwordLength) + passwordLength;
    _rawData.formatted.type = HTONS(BPOpCodeLoginUser);
    _rawData.formatted.data._241.passwordLength = HTONLL(passwordLength);
    std::memset(_rawData.formatted.data._241.username, 0, sizeof(_rawData.formatted.data._241.username));
    std::memcpy(_rawData.formatted.data._241.username, username.c_str(), username.length());
    std::memcpy(_rawData.formatted.data._241.password, password.c_str(), passwordLength);
}

void b12software::babel::BabelPacket::createUpdateUserPacket(long userId, const std::string &username, long birthDate,
                                                             const std::string &email, const std::string &password)
{
    size_t passwordLength = password.length();
    size_t emailLength = email.length();
    if (passwordLength + emailLength > sizeof(_rawData.formatted.data._242.emailAndPassword))
        return;
    if (username.length() > 10)
        return;
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._242.userId) + sizeof(_rawData.formatted.data._242.username) + sizeof(_rawData.formatted.data._242.birthDate) + sizeof(_rawData.formatted.data._242.emailLength) + sizeof(_rawData.formatted.data._242.passwordLength) + passwordLength + emailLength;
    _rawData.formatted.type = HTONS(BPOpCodeUpdateUser);
    _rawData.formatted.data._242.userId = HTONLL(userId);
    std::memset(_rawData.formatted.data._242.username, 0, sizeof(_rawData.formatted.data._242.username));
    std::memcpy(_rawData.formatted.data._242.username, username.c_str(), username.length());
    _rawData.formatted.data._242.birthDate = HTONLL(birthDate);
    _rawData.formatted.data._242.passwordLength = HTONLL(passwordLength);
    _rawData.formatted.data._242.emailLength = HTONLL(emailLength);
    std::memcpy(_rawData.formatted.data._242.emailAndPassword, email.c_str(), emailLength);
    std::memcpy(reinterpret_cast<void *>(reinterpret_cast<intptr_t>(_rawData.formatted.data._242.emailAndPassword) + emailLength), password.c_str(), passwordLength);
}

void b12software::babel::BabelPacket::createRequestContactRequestPacket(long userId)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    _rawData.formatted.type = HTONS(BPOpCodeRequestContactRequest);
    _rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId = HTONLL(userId);
}

void b12software::babel::BabelPacket::createRequestUserContactResponsePacket(long userId, bool response)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._203_212_301);
    _rawData.formatted.type = HTONS(BPOpCodeRequestUserContactResponse);
    _rawData.formatted.data._203_212_301.targetId = HTONLL(userId);
    _rawData.formatted.data._203_212_301.response = response;
}

void b12software::babel::BabelPacket::createDeletedUserContactListResponsePacket(long userId)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    _rawData.formatted.type = HTONS(BPOpCodeDeletedUserContactListResponse);
    _rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId = HTONLL(userId);
}

void b12software::babel::BabelPacket::createListContactListResponsePacket(const std::vector<long> &ids)
{
    size_t nbIds = ids.size();
    if (nbIds * sizeof(uint64_t) > sizeof(_rawData.formatted.data._303_305.contactIds))
        return;
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._303_305.nbContacts) + nbIds * sizeof(uint64_t);
    _rawData.formatted.type = HTONS(BPOpCodeListContactListResponse);
    _rawData.formatted.data._303_305.nbContacts = HTONLL(nbIds);
    for (size_t i = 0; i < nbIds; i++) {
        _rawData.formatted.data._303_305.contactIds[i] = HTONLL(ids[i]);
    }
}

void b12software::babel::BabelPacket::createListContactInfoResponsePacket(long userId, const std::string &username,
                                                                          long birthDate, const std::string &email)
{
    size_t emailLength = email.length();
    if (username.length() > 10)
        return;
    if (emailLength > sizeof(_rawData.formatted.data._304.email))
        return;
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._304.userId) + sizeof(_rawData.formatted.data._304.username) + sizeof(_rawData.formatted.data._304.emailLength) + sizeof(_rawData.formatted.data._304.birthDate) + emailLength;
    _rawData.formatted.type = HTONS(BPOpCodeListContactInfoResponse);
    _rawData.formatted.data._304.userId = HTONLL(userId);
    std::memset(_rawData.formatted.data._304.username, 0, sizeof(_rawData.formatted.data._304.username));
    std::memcpy(_rawData.formatted.data._304.username, username.c_str(), username.length());
    _rawData.formatted.data._304.birthDate = HTONLL(birthDate);
    _rawData.formatted.data._304.emailLength = HTONLL(emailLength);
    std::memcpy(_rawData.formatted.data._304.email, email.c_str(), emailLength);
}

void b12software::babel::BabelPacket::createListContactRequestListResponsePacket(const std::vector<long> &ids)
{
    size_t nbIds = ids.size();
    if (nbIds * sizeof(uint64_t) > sizeof(_rawData.formatted.data._303_305.contactIds))
        return;
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._303_305.nbContacts) + nbIds * sizeof(uint64_t);
    _rawData.formatted.type = HTONS(BPOpCodeListContactRequestListResponse);
    _rawData.formatted.data._303_305.nbContacts = HTONLL(nbIds);
    for (size_t i = 0; i < nbIds; i++) {
        _rawData.formatted.data._303_305.contactIds[i] = HTONLL(ids[i]);
    }
}

void b12software::babel::BabelPacket::createReceivedCallPacket(long userId, int ipv4, int port)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._310);
    _rawData.formatted.type = HTONS(BPOpCodeReceivedCall);
    _rawData.formatted.data._310.callerUserId = HTONLL(userId);
    _rawData.formatted.data._310.ipv4Addr = HTONL(ipv4);
    _rawData.formatted.data._310.port = HTONL(port);
}

void b12software::babel::BabelPacket::createCallUserResponsePacket(long userId, bool response, int ipv4, int port)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._311);
    _rawData.formatted.type = HTONS(BPOpCodeCallUserResponse);
    _rawData.formatted.data._311.answer = response;
    _rawData.formatted.data._311.calledUserId = HTONLL(userId);
    _rawData.formatted.data._311.ipv4Addr = HTONL(ipv4);
    _rawData.formatted.data._311.port = HTONL(port);
}

void b12software::babel::BabelPacket::createUserHangedUpPacket(long userId)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    _rawData.formatted.type = HTONS(BPOpCodeUserHangedUp);
    _rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId = HTONLL(userId);
}

void b12software::babel::BabelPacket::createReceivedMessagePacket(long userId, const std::string &message)
{
    size_t messageSize = message.length();
    if (messageSize > sizeof(_rawData.formatted.data._220_320.msg))
        return;
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._220_320.targetId) + sizeof(_rawData.formatted.data._220_320.msgLength) + messageSize;
    _rawData.formatted.type = HTONS(BPOpCodeReceivedMessage);
    _rawData.formatted.data._220_320.targetId = HTONLL(userId);
    _rawData.formatted.data._220_320.msgLength = HTONLL(messageSize);
    std::memcpy(_rawData.formatted.data._220_320.msg, message.c_str(), messageSize);
}

void b12software::babel::BabelPacket::createRetrieveMessageListResponsePacket(long userId, const std::vector<std::pair<uint64_t, std::string>> &messages)
{
    size_t nbMessages = messages.size();
    size_t totSize = 0;
    for (auto &msg : messages)
        totSize += (sizeof(uint64_t) + sizeof(uint64_t) + msg.second.length());
    if (totSize > sizeof(_rawData.formatted.data._321.messages))
        return;
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._321.userId) + sizeof(_rawData.formatted.data._321.nbMessages) + totSize;
    _rawData.formatted.type = HTONS(BPOpCodeRetrieveMessageListResponse);
    _rawData.formatted.data._321.userId = HTONLL(userId);
    _rawData.formatted.data._321.nbMessages = HTONLL(nbMessages);
    size_t byteWritten = 0;
    intptr_t ptr = reinterpret_cast<intptr_t>(_rawData.formatted.data._321.messages);
    for (auto &msg : messages) {
        uint64_t msgUser = HTONLL(msg.first);
        uint64_t msgLenNetwork = HTONLL(msg.second.length());
        std::memcpy(reinterpret_cast<void *>(ptr + byteWritten), &msgUser, sizeof(msgUser));
        byteWritten += sizeof(msgLenNetwork);
        std::memcpy(reinterpret_cast<void *>(ptr + byteWritten), &msgLenNetwork, sizeof(msgLenNetwork));
        byteWritten += sizeof(msgLenNetwork);
        std::memcpy(reinterpret_cast<void *>(ptr + byteWritten), msg.second.c_str(), msg.second.length());
        byteWritten += msg.second.length();
    }
}

void b12software::babel::BabelPacket::createUserAuthResponsePacket(long userId)
{
    _valid = true;
    _packetSize = sizeof(BabelProtocolOpCode) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    _rawData.formatted.type = HTONS(BPOpCodeUserAuthResponse);
    _rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId = HTONLL(userId);
}

b12software::babel::BabelPacket::BabelProtocolOpCode b12software::babel::BabelPacket::getPacketOpCode() const
{
    if (!isValid())
        return BPOpCodeNone;
    return static_cast<BabelProtocolOpCode>(NTOHS(_rawData.formatted.type));
}

void b12software::babel::BabelPacket::extractSendBinaryResponsePacket(bool &response, std::string &description)
{
    response = _rawData.formatted.data._100.response;
    size_t msgLen = NTOHLL(_rawData.formatted.data._100.msgLength);
    description.clear();
    description.assign(_rawData.formatted.data._100.msg, msgLen);
}

void b12software::babel::BabelPacket::extractAddUserContactListPacket(long &userId)
{
    userId = NTOHLL(_rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId);
}

void b12software::babel::BabelPacket::extractDeleteUserContactListPacket(long &userId)
{
    userId = NTOHLL(_rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId);
}

void b12software::babel::BabelPacket::extractSendResponseContactRequestPacket(long &userId, bool &response)
{
    userId = NTOHLL(_rawData.formatted.data._203_212_301.targetId);
    response = _rawData.formatted.data._203_212_301.response;
}

void b12software::babel::BabelPacket::extractListContactInfoPacket(long &userId)
{
    userId = NTOHLL(_rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId);
}

void b12software::babel::BabelPacket::extractStartCallPacket(long &userId)
{
    userId = NTOHLL(_rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId);
}

void b12software::babel::BabelPacket::extractHangupCallPacket(long &userId)
{
    userId = NTOHLL(_rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId);
}

void b12software::babel::BabelPacket::extractSendResponseCallPacket(long &userId, bool &response)
{
    userId = NTOHLL(_rawData.formatted.data._203_212_301.targetId);
    response = _rawData.formatted.data._203_212_301.response;
}

void b12software::babel::BabelPacket::extractSendMessagePacket(long &userId, std::string &message)
{
    userId = NTOHLL(_rawData.formatted.data._220_320.targetId);
    size_t msgLen = NTOHLL(_rawData.formatted.data._220_320.msgLength);
    message.clear();
    message.assign(_rawData.formatted.data._220_320.msg, msgLen);
}

void b12software::babel::BabelPacket::extractRetrieveMessageListPacket(long &userId, long &startTimeStamp,
                                                                       long &endTimeStamp)
{
    userId = NTOHLL(_rawData.formatted.data._221.targetId);
    startTimeStamp = NTOHLL(_rawData.formatted.data._221.startTimeStamp);
    endTimeStamp = NTOHLL(_rawData.formatted.data._221.endTimeStamp);
}

void b12software::babel::BabelPacket::extractRegisterNewUserPacket(std::string &username, long &birthDate,
                                                                   std::string &email, std::string &password)
{
    birthDate = NTOHLL(_rawData.formatted.data._240.birthDate);
    size_t pwdSize = NTOHLL(_rawData.formatted.data._240.passwordLength);
    size_t emailSize = NTOHLL(_rawData.formatted.data._240.emailLength);
    username.clear();
    size_t usernameSize = 0;
    for (; _rawData.formatted.data._240.username[usernameSize] != 0 && usernameSize < 10; usernameSize++);
    username.assign(_rawData.formatted.data._240.username, usernameSize);
    email.clear();
    email.assign(_rawData.formatted.data._240.emailAndPassword, emailSize);
    password.clear();
    password.assign(_rawData.formatted.data._240.emailAndPassword, emailSize, pwdSize);
}

void b12software::babel::BabelPacket::extractLoginUserPacket(std::string &username, std::string &password)
{
    size_t passwordSize = NTOHLL(_rawData.formatted.data._241.passwordLength);
    username.clear();
    size_t usernameSize = 0;
    for (; _rawData.formatted.data._241.username[usernameSize] != 0 && usernameSize < 10; usernameSize++);
    username.assign(_rawData.formatted.data._241.username, usernameSize);
    password.clear();
    password.assign(_rawData.formatted.data._241.password, passwordSize);
}

void b12software::babel::BabelPacket::extractUpdateUserPacket(long &userId, std::string &username, long &birthDate,
                                                              std::string &email, std::string &password)
{
    userId = NTOHLL(_rawData.formatted.data._242.userId);
    username.clear();
    size_t usernameSize = 0;
    for (; _rawData.formatted.data._242.username[usernameSize] != 0 && usernameSize < 10; usernameSize++);
    username.assign(_rawData.formatted.data._242.username, usernameSize);
    birthDate = NTOHLL(_rawData.formatted.data._242.birthDate);
    size_t pwdSize = NTOHLL(_rawData.formatted.data._242.passwordLength);
    size_t emailSize = NTOHLL(_rawData.formatted.data._242.emailLength);
    email.clear();
    email.assign(_rawData.formatted.data._242.emailAndPassword, emailSize);
    password.clear();
    password.assign(_rawData.formatted.data._242.emailAndPassword, emailSize, pwdSize);
}

void b12software::babel::BabelPacket::extractRequestContactRequestPacket(long &userId)
{
    userId = NTOHLL(_rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId);
}

void b12software::babel::BabelPacket::extractRequestUserContactResponsePacket(long &userId, bool &response)
{
    userId = NTOHLL(_rawData.formatted.data._203_212_301.targetId);
    response = _rawData.formatted.data._203_212_301.response;
}

void b12software::babel::BabelPacket::extractDeletedUserContactListResponsePacket(long &userId)
{
    userId = NTOHLL(_rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId);
}

void b12software::babel::BabelPacket::extractListContactListResponsePacket(std::vector<long> &ids)
{
    size_t nbIds = NTOHLL(_rawData.formatted.data._303_305.nbContacts);
    ids.clear();
    for (size_t i = 0; i < nbIds; i++) {
        ids.push_back(NTOHLL(_rawData.formatted.data._303_305.contactIds[i]));
    }
}

void b12software::babel::BabelPacket::extractListContactInfoResponsePacket(long &userId, std::string &username,
    long &birthDate, std::string &email)
{
    userId = NTOHLL(_rawData.formatted.data._304.userId);
    birthDate = NTOHLL(_rawData.formatted.data._304.birthDate);
    username.clear();
    size_t usernameSize = 0;
    for (; _rawData.formatted.data._304.username[usernameSize] != 0 && usernameSize < 10; usernameSize++);
    username.assign(_rawData.formatted.data._304.username, usernameSize);
    size_t emailSize = NTOHLL(_rawData.formatted.data._304.emailLength);
    email.clear();
    email.assign(_rawData.formatted.data._304.email, emailSize);
}

void b12software::babel::BabelPacket::extractListContactRequestListResponsePacket(std::vector<long> &ids)
{
    size_t nbIds = NTOHLL(_rawData.formatted.data._303_305.nbContacts);
    ids.clear();
    for (size_t i = 0; i < nbIds; i++) {
        ids.push_back(NTOHLL(_rawData.formatted.data._303_305.contactIds[i]));
    }
}

void b12software::babel::BabelPacket::extractReceivedCallPacket(long &userId, int &ipv4, int &port)
{
    userId = NTOHLL(_rawData.formatted.data._310.callerUserId);
    ipv4 = NTOHL(_rawData.formatted.data._310.ipv4Addr);
    port = NTOHL(_rawData.formatted.data._310.port);
}

void b12software::babel::BabelPacket::extractCallUserResponsePacket(long &userId, bool &response, int &ipv4, int &port)
{
    userId = NTOHLL(_rawData.formatted.data._311.calledUserId);
    response = _rawData.formatted.data._311.answer;
    ipv4 = NTOHL(_rawData.formatted.data._311.ipv4Addr);
    port = NTOHL(_rawData.formatted.data._311.port);
}

void b12software::babel::BabelPacket::extractUserHangedUpPacket(long &userId)
{
    userId = NTOHLL(_rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId);
}

void b12software::babel::BabelPacket::extractReceivedMessagePacket(long &userId, std::string &message)
{
    userId = NTOHLL(_rawData.formatted.data._220_320.targetId);
    size_t msgLen = NTOHLL(_rawData.formatted.data._220_320.msgLength);
    message.clear();
    message.assign(_rawData.formatted.data._220_320.msg, msgLen);
}

void b12software::babel::BabelPacket::extractRetrieveMessageListResponsePacket(long &userId,
                                                                               std::vector<std::pair<uint64_t, std::string>> &messages)
{
    userId = NTOHLL(_rawData.formatted.data._321.userId);
    size_t nbMessages = NTOHLL(_rawData.formatted.data._321.nbMessages);
    messages.clear();
    size_t extractedBytes = 0;
    intptr_t ptr = reinterpret_cast<intptr_t>(_rawData.formatted.data._321.messages);
    for (size_t i = 0; i < nbMessages; i++) {
        size_t msgUser = NTOHLL(*reinterpret_cast<uint64_t *>(ptr + extractedBytes));
        extractedBytes += sizeof(uint64_t);
        size_t msgLen = NTOHLL(*reinterpret_cast<uint64_t *>(ptr + extractedBytes));
        extractedBytes += sizeof(uint64_t);
        messages.emplace_back(std::make_pair(msgUser, std::string(reinterpret_cast<const char *>(ptr + extractedBytes), msgLen)));
        extractedBytes += msgLen;
    }
}

void b12software::babel::BabelPacket::extractUserAuthResponsePacket(long &userId)
{
    userId = NTOHLL(_rawData.formatted.data._200_201_204_210_211_300_302_312_340.targetId);
}

size_t b12software::babel::BabelPacket::buildSendBinaryResponseFromData(const void *data, size_t size)
{
    clear();
    intptr_t ptr = reinterpret_cast<intptr_t>(data);
    const uint64_t *descriptionLengthPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t) + sizeof(_rawData.formatted.data._100.response));
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._100.response) + sizeof(_rawData.formatted.data._100.msgLength);
    if (size < neededSize)
        return 0;
    neededSize += NTOHLL(*descriptionLengthPtr);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildAddUserContactListFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildDeleteUserContactListFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildListContactListFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildSendResponseContactRequestFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._203_212_301);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildListContactInfoFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildCreateListContactRequestListPacketFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildStartCallFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildHangupCallFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildSendResponseCallFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._203_212_301);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildSendMessageFromData(const void *data, size_t size)
{
    clear();
    intptr_t ptr = reinterpret_cast<intptr_t>(data);
    const uint64_t *msgLengthPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t) + sizeof(_rawData.formatted.data._220_320.targetId));
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._220_320.targetId) + sizeof(_rawData.formatted.data._220_320.msgLength);
    if (size < neededSize)
        return 0;
    neededSize += NTOHLL(*msgLengthPtr);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildRetrieveMessageListFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._221);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildRegisterNewUserFromData(const void *data, size_t size)
{
    clear();
    intptr_t ptr = reinterpret_cast<intptr_t>(data);
    const uint64_t *emailLenghPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t) + sizeof(_rawData.formatted.data._240.username) + sizeof(_rawData.formatted.data._240.birthDate));
    const uint64_t *passwordLenghPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t) + sizeof(_rawData.formatted.data._240.username) + sizeof(_rawData.formatted.data._240.birthDate) + sizeof(_rawData.formatted.data._240.emailLength));
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._240.username) + sizeof(_rawData.formatted.data._240.birthDate) + sizeof(_rawData.formatted.data._240.emailLength) + sizeof(_rawData.formatted.data._240.passwordLength);
    if (size < neededSize)
        return 0;
    neededSize += (NTOHLL(*emailLenghPtr) + NTOHLL(*passwordLenghPtr));
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildLoginUserFromData(const void *data, size_t size)
{
    clear();
    intptr_t ptr = reinterpret_cast<intptr_t>(data);
    const uint64_t *passwordLenghPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t) + sizeof(_rawData.formatted.data._241.username));
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._241.username) + sizeof(_rawData.formatted.data._241.passwordLength);
    if (size < neededSize)
        return 0;
    neededSize += NTOHLL(*passwordLenghPtr);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildUpdateUserFromData(const void *data, size_t size)
{
    clear();
    intptr_t ptr = reinterpret_cast<intptr_t>(data);
    const uint64_t *emailLenghPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t) + sizeof(_rawData.formatted.data._242.userId) + sizeof(_rawData.formatted.data._242.username) + sizeof(_rawData.formatted.data._242.birthDate));
    const uint64_t *passwordLenghPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t) + sizeof(_rawData.formatted.data._242.userId) + sizeof(_rawData.formatted.data._242.username) + sizeof(_rawData.formatted.data._242.birthDate) + sizeof(_rawData.formatted.data._242.emailLength));
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._242.userId) + sizeof(_rawData.formatted.data._242.username) + sizeof(_rawData.formatted.data._242.birthDate) + sizeof(_rawData.formatted.data._242.emailLength) + sizeof(_rawData.formatted.data._242.passwordLength);
    if (size < neededSize)
        return 0;
    neededSize += (NTOHLL(*emailLenghPtr) + NTOHLL(*passwordLenghPtr));
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildRequestContactRequestFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildRequestUserContactResponseFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._203_212_301);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildDeletedUserContactListResponseFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildListContactListResponseFromData(const void *data, size_t size)
{
    clear();
    intptr_t ptr = reinterpret_cast<intptr_t>(data);
    const uint64_t *nbContactPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t));
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._303_305.nbContacts);
    if (size < neededSize)
        return 0;
    neededSize += (NTOHLL(*nbContactPtr) * sizeof(uint64_t));
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildListContactInfoResponseFromData(const void *data, size_t size)
{
    clear();
    intptr_t ptr = reinterpret_cast<intptr_t>(data);
    const uint64_t *emailLengthPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t) + sizeof(_rawData.formatted.data._304.userId));
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._304.userId) + sizeof(_rawData.formatted.data._304.emailLength) + sizeof(_rawData.formatted.data._304.username) + sizeof(_rawData.formatted.data._304.birthDate);
    if (size < neededSize)
        return 0;
    neededSize += NTOHLL(*emailLengthPtr);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildListContactRequestListResponseFromData(const void *data, size_t size)
{
    clear();
    intptr_t ptr = reinterpret_cast<intptr_t>(data);
    const uint64_t *nbContactPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t));
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._303_305.nbContacts);
    if (size < neededSize)
        return 0;
    neededSize += (NTOHLL(*nbContactPtr) * sizeof(uint64_t));
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildReceivedCallFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._310);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildCallUserResponseFromData(const void *data, size_t size)
{
    clear();

    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._311);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildUserHangedUpFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildReceivedMessageFromData(const void *data, size_t size)
{
    clear();
    intptr_t ptr = reinterpret_cast<intptr_t>(data);
    const uint64_t *msgLengthPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t) + sizeof(_rawData.formatted.data._220_320.targetId));
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._220_320.targetId) + sizeof(_rawData.formatted.data._220_320.msgLength);
    if (size < neededSize)
        return 0;
    neededSize += NTOHLL(*msgLengthPtr);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildRetrieveMessageListResponseFromData(const void *data, size_t size)
{
    clear();
    intptr_t ptr = reinterpret_cast<intptr_t>(data);
    const uint64_t *nbMessagesPtr = reinterpret_cast<const uint64_t *>(ptr + sizeof(uint16_t) + sizeof(_rawData.formatted.data._321.userId));
    const uint64_t *currentMsgUserPtr = nullptr;
    const uint64_t *currentMsgSizePtr = nullptr;
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._321.userId) + sizeof(_rawData.formatted.data._321.nbMessages);
    if (size < neededSize)
        return 0;
    size_t nbMessages = NTOHLL(*nbMessagesPtr);
    for (size_t i = 0; i < nbMessages; i++) {
        currentMsgUserPtr = reinterpret_cast<const uint64_t *>(ptr + neededSize);
        neededSize += sizeof(uint64_t);
        currentMsgSizePtr = reinterpret_cast<const uint64_t *>(ptr + neededSize);
        neededSize += sizeof(uint64_t);
        if (size < neededSize)
            return 0;
        neededSize += NTOHLL(*currentMsgSizePtr);
        if (size < neededSize)
            return 0;
    }
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}

size_t b12software::babel::BabelPacket::buildUserAuthResponseFromData(const void *data, size_t size)
{
    clear();
    size_t neededSize = sizeof(uint16_t) + sizeof(_rawData.formatted.data._200_201_204_210_211_300_302_312_340);
    if (size < neededSize)
        return 0;
    std::memcpy(_rawData.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
    return _packetSize;
}
