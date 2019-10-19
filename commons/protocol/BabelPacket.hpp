/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** BabelPacket.hpp
*/

/* Created the 01/10/2019 at 16:02 by julian.frabel@epitech.eu */

#ifndef BABEL_BABELPACKET_HPP
#define BABEL_BABELPACKET_HPP

#include <map>
#include <chrono>
#include <vector>
#include "network/IPacket.hpp"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <ws2tcpip.h>
    #include <winsock2.h>

    #define HTONS(x) (htons(x))
    #define NTOHS(x) (ntohs(x))

    #define HTONL(x) (htonl(x))
    #define NTOHL(x) (ntohl(x))

    #define HTONLL(x) (htonll(x))
    #define NTOHLL(x) (ntohll(x))

    #define PACKED(__DECLARATION__) __pragma(pack(push, 1)) __DECLARATION__ __pragma(pack(pop))
#else
    #include <sys/types.h>
    #include <netinet/in.h>

    #define HTONS(x) (htons(x))
    #define NTOHS(x) (ntohs(x))

    #define HTONL(x) (htonl(x))
    #define NTOHL(x) (ntohl(x))

    #define HTONLL(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
    #define NTOHLL(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))

    #define PACKED(__DECLARATION__) __DECLARATION__ __attribute__((packed))
#endif

/*!
 * @namespace b12software
 * @brief Main namespace for all b12 software
 */
namespace b12software {

    /*!
     * @namespace b12software::babel
     * @brief Main namespace for the babel project
     */
    namespace babel {

        /*!
         * @class BabelPacket
         * @brief A class used to easely create babel protocol tcp packets
         */
        class BabelPacket : public network::IPacket {
        public:
            static constexpr int udpPort = 6942;
            /*!
             * @enum BabelProtocolOpCode
             * @brief Operation code supported by the babel protocol
             */
            enum BabelProtocolOpCode : uint16_t {
                BPOpCodeNone = 0,
                BPOpCodeSendBinaryResponse = 100,
                BPOpCodeAddUserContactList = 200,
                BPOpCodeDeleteUserContactList = 201,
                BPOpCodeListContactList = 202,
                BPOpCodeSendResponseContactRequest = 203,
                BPOpCodeListContactInfo = 204,
                BPOpCodeListContactRequestList = 205,
                BPOpCodeStartCall = 210,
                BPOpCodeHangupCall = 211,
                BPOpCodeSendResponseCall = 212,
                BPOpCodeSendMessage = 220,
                BPOpCodeRetrieveMessageList = 221,
                BPOpCodeRegisterNewUser = 240,
                BPOpCodeLoginUser = 241,
                BPOpCodeUpdateUser = 242,
                BPOpCodeRequestContactRequest = 300,
                BPOpCodeRequestUserContactResponse = 301,
                BPOpCodeDeletedUserContactListResponse = 302,
                BPOpCodeListContactListResponse = 303,
                BPOpCodeListContactInfoResponse = 304,
                BPOpCodeListContactRequestListResponse = 305,
                BPOpCodeReceivedCall = 310,
                BPOpCodeCallUserResponse = 311,
                BPOpCodeUserHangedUp = 312,
                BPOpCodeReceivedMessage = 320,
                BPOpCodeRetrieveMessageListResponse = 321,
                BPOpCodeUserAuthResponse = 340,
            };

        private:
            /*!
             * @union BabelProtocolPacketData
             * @brief Data arranged for the babel protocol
             *
             * This structure is not documented as it is only used to map the protocol to a byte array automatically.
             * Set the type to the correct op code, fill the data._request accordingly and use byteArray.
             * This structure is used internally and you should not try to edit it by hand, use the helper functions.
             */
            union BabelProtocolPacketData {
				PACKED(struct {
                    uint16_t type;
                    union {
						PACKED(struct {
                            uint8_t response;
                            uint64_t msgLength;
                            char msg[65535 - sizeof(uint16_t) - sizeof(uint8_t) - sizeof(uint64_t)];
                        }) _100;
						PACKED(struct {
                            uint64_t targetId;
                        }) _200_201_204_210_211_300_302_312_340;
						PACKED(struct {

                        }) _202_205;
						PACKED(struct {
                            uint64_t targetId;
                            uint8_t response;
                        }) _203_212_301;
						PACKED(struct {
                            uint64_t targetId;
                            uint64_t msgLength;
                            char msg[65535 - sizeof(uint16_t) - sizeof(uint64_t) - sizeof(uint64_t)];
                        }) _220_320;
						PACKED(struct {
                            uint64_t targetId;
                            uint64_t startTimeStamp;
                            uint64_t endTimeStamp;
                        }) _221;
						PACKED(struct {
                            char username[10];
                            uint64_t birthDate;
                            uint64_t emailLength;
                            uint64_t passwordLength;
                            char emailAndPassword[65535 - sizeof(uint16_t) - 10 - sizeof(uint64_t) - sizeof(uint64_t) - sizeof(uint64_t)];
                        }) _240;
						PACKED(struct {
                            char username[10];
                            uint64_t passwordLength;
                            char password[65535 - sizeof(uint16_t) - 10 - - sizeof(uint64_t)];
                        }) _241;
						PACKED(struct {
                            uint64_t userId;
                            char username[10];
                            uint64_t birthDate;
                            uint64_t emailLength;
                            uint64_t passwordLength;
                            char emailAndPassword[65535 - sizeof(uint16_t) - sizeof(uint64_t) - 10 - sizeof(uint64_t) - sizeof(uint64_t) - sizeof(uint64_t)];
                        }) _242;
						PACKED(struct {
                            uint64_t nbContacts;
                            uint64_t contactIds[(65535 - sizeof(uint16_t) - sizeof(uint64_t)) / sizeof(uint64_t)];
                        }) _303_305;
						PACKED(struct {
						    uint64_t userId;
                            uint64_t emailLength;
                            char username[10];
                            uint64_t birthDate;
                            char email[65535 - sizeof(uint16_t) - sizeof(uint64_t) - 10 - sizeof(uint64_t)];
                        }) _304;
						PACKED(struct {
                            uint64_t callerUserId;
                            uint32_t ipv4Addr;
                            uint32_t port;
                        }) _310;
						PACKED(struct {
                            uint64_t calledUserId;
                            uint8_t answer;
                            uint32_t ipv4Addr;
                            uint32_t port;
                        }) _311;
						PACKED(struct {
                            uint64_t userId;
                            uint64_t nbMessages;
                            char messages[65535 - sizeof(uint16_t) - sizeof(uint64_t) - sizeof(uint64_t)]; //interpret this array as uint64_t, uint64_t then a message of len in the second uint64_t
                        }) _321;
                    } data;
                }) formatted;
                unsigned char byteArray[65535];
            };

        public:
            /*!
             * @brief ctor
             */
            BabelPacket();
            /*!
             * @brief dtor
             */
            ~BabelPacket() override;

        public:
            size_t getSize() const override;
            const void *getData() const override;
            size_t setData(const void *data, size_t size) override;
            void clear() override;
            bool isValid() const override;

        public:
            /*!
             * @brief Create a 100 request packet
             * @param response The response to use
             * @param description The description to set
             *
             * The 100 packet is used to indicate if a command was successfully executed or not and give a description
             */
            void createSendBinaryResponsePacket(bool response, const std::string &description);
            /*!
             * @brief Create a 200 request packet
             * @param userId The user add to the user list
             *
             * The 200 packet is used to ask for a contact to be added to the contact list
             */
            void createAddUserContactListPacket(long userId);
            /*!
             * @brief Create a 201 request packet
             * @param userId The user id to delete from the contact list
             *
             * The 201 packet is used to delete a contact from the connected client contact list
             */
            void createDeleteUserContactListPacket(long userId);
            /*!
             * @brief Create a 202 request packet
             *
             * The 202 packet is used to retrieve all the contact ids in the contact list
             */
            void createListContactListPacket();
            /*!
             * @brief Create a 203 request packet
             * @param userId The user id to accept or deny the invitation
             * @param response The response (true or false)
             *
             * The 203 packet is used to accept or deny a contact request
             */
            void createSendResponseContactRequestPacket(long userId, bool response);
            /*!
             * @brief Create a 204 request packet
             * @param userId The user id to retrieve information about
             *
             * The 204 packet is used to retrieve information about a specific contact id
             */
            void createListContactInfoPacket(long userId);
            /*!
             * @brief Create a 205 request packet
             *
             * The 205 packet is used to retrieve all the contact ids in the contact request list
             */
            void createListContactRequestListPacket();
            /*!
             * @brief Create a 210 request packet
             * @param userId The user id to call
             *
             * The 210 packet is used to start a call with a user
             */
            void createStartCallPacket(long userId);
            /*!
             * @brief Create a 211 request packet
             * @param userId The user id to hangup with
             *
             * The 211 packet is used to end a call with a user
             */
            void createHangupCallPacket(long userId);
            /*!
             * @brief Create a 212 request packet
             * @param userId The user id to accept or deny a call with
             * @param response The response to give (true or false)
             *
             * The 212 packet is used to accept or deny a call
             */
            void createSendResponseCallPacket(long userId, bool response);
            /*!
             * @brief Create a 220 request packet
             * @param userId The user id to which send the message
             * @param message The message to send
             *
             * The 220 packet is used to send a message to a user
             */
            void createSendMessagePacket(long userId, const std::string &message);
            /*!
             * @brief Create a 221 request packet
             * @param userId The user id with which the messages should be retrieved
             * @param startTimeStamp The start time stamp
             * @param endTimeStamp The end time stamp
             *
             * The 221 packet is used to retrieve all messages with a user between two time stamps
             */
            void createRetrieveMessageListPacket(long userId, long startTimeStamp, long endTimeStamp);
            /*!
             * @brief Create a 240 request packet
             * @param username The username of the new user (should be unique)
             * @param birthDate The birth date of the new user
             * @param email The email of the new user
             * @param password The password of the new user
             *
             * The 240 packet is used to signup new users
             */
            void createRegisterNewUserPacket(const std::string &username, long birthDate, const std::string &email, const std::string &password);
            /*!
             * @brief Create a 241 request packet
             * @param username The username of the user who try to log in
             * @param password The password of the user who try to log in
             *
             * The 241 packet is used to authenticate yourself on the server
             */
            void createLoginUserPacket(const std::string &username, const std::string &password);
            /*!
             * @brief Create a 242 request packet
             * @param userId The user id of the user to update
             * @param username The new username
             * @param birthDate The new birth date
             * @param email The new email
             * @param password The new password
             *
             * The 242 packet is used to update a user information
             */
            void createUpdateUserPacket(long userId, const std::string &username, long birthDate, const std::string &email, const std::string &password);
            /*!
             * @brief Create a 300 response packet
             * @param userId The user if from which the contact request comes
             *
             * The 300 packet is used by the server to indicate that someone asked you in contact
             */
            void createRequestContactRequestPacket(long userId);
            /*!
             * @brief Create a 301 response packet
             * @param userId The user id that answered to the contact request
             * @param response The response of this user (true or false)
             *
             * The 301 packet is used by the server to indicate to a user that someone accepted / denied a friend request
             */
            void createRequestUserContactResponsePacket(long userId, bool response);
            /*!
             * @brief Create a 302 response packet
             * @param userId The user id that has deleted the user from it's list
             *
             * The 302 packet is used by the server to indicate to a user that someone removed him from the contact list
             */
            void createDeletedUserContactListResponsePacket(long userId);
            /*!
             * @brief Create a 303 response packet
             * @param ids The ids in the contact list
             *
             * The 303 packet is used by the server to answer to a contact list request
             */
            void createListContactListResponsePacket(const std::vector<long> &ids);
            /*!
             * @brief Create a 304 response packet
             * @param userId The id of the user this packet is about
             * @param username The username of the requested user
             * @param birthDate The birth date of the requested user
             * @param email The email of the requested user
             *
             * The 304 packet is used by the server to answer to a contact infos request
             */
            void createListContactInfoResponsePacket(long userId, const std::string &username, long birthDate, const std::string &email);
            /*!
             * @brief Create a 305 response packet
             * @param ids The ids in the contact request list
             *
             * The 305 packet is used by the server to answer to a contact request list request
             */
            void createListContactRequestListResponsePacket(const std::vector<long> &ids);
            /*!
             * @brief Create a 310 response packet
             * @param userId The user id of the user who called
             * @param ipv4 The ip address of the caller
             * @param port The port of the caller
             *
             * The 310 packet is used to indicate that the user is being called by someone
             */
            void createReceivedCallPacket(long userId, int ipv4, int port);
            /*!
             * @brief Create a 311 response packet
             * @param userId The id of the user who was called
             * @param response The response of the user who was called
             * @param ipv4 The ip address of the user who was called
             * @param port The port of the user who was called
             *
             * The 311 packet is used by the server to indicate to a caller that the user accepted / denied the call
             */
            void createCallUserResponsePacket(long userId, bool response, int ipv4, int port);
            /*!
             * @brief create a 312 response packet
             * @param userId The id of the user who hanged up
             *
             * The 312 packet is used by the server to indicate that a remote user ended the call
             */
            void createUserHangedUpPacket(long userId);
            /*!
             * @brief Create a 320 response packet
             * @param userId The id of the user who sent the message
             * @param message The message that was sent
             *
             * The 320 packet is used by the server to indicate to a user that he received a message
             */
            void createReceivedMessagePacket(long userId, const std::string &message);
            /*!
             * @brief Create a 321 response packet
             * @param userId The user of the messages list
             * @param messages The messages that were sent/received to/from the user in first user
             *
             * The 321 packet is used by the server to response to a messsage list
             */
            void createRetrieveMessageListResponsePacket(long userId, const std::vector<std::pair<uint64_t, std::string>> &messages);
            /*!
             * @brief Create a 340 response packet
             * @param userId The user id who was logged/updated
             *
             * The 340 packet is used to send the user id who was logged in or updated
             */
            void createUserAuthResponsePacket(long userId);

            /*!
             * @brief Get the operation code of this packet
             * @return The operation code of this packet
             */
            BabelProtocolOpCode getPacketOpCode() const;

            /*!
             * @brief Extract a 100 request packet
             * @param response The response to use
             * @param description The description to set
             *
             * The 100 packet is used to indicate if a command was successfully executed or not and give a description
             */
            void extractSendBinaryResponsePacket(bool &response, std::string &description);
            /*!
             * @brief Extract a 200 request packet
             * @param userId The user add to the user list
             *
             * The 200 packet is used to ask for a contact to be added to the contact list
             */
            void extractAddUserContactListPacket(long &userId);
            /*!
             * @brief Extract a 201 request packet
             * @param userId The user id to delete from the contact list
             *
             * The 201 packet is used to delete a contact from the connected client contact list
             */
            void extractDeleteUserContactListPacket(long &userId);
            /*!
             * @brief Extract a 203 request packet
             * @param userId The user id to accept or deny the invitation
             * @param response The response (true or false)
             *
             * The 203 packet is used to accept or deny a contact request
             */
            void extractSendResponseContactRequestPacket(long &userId, bool &response);
            /*!
             * @brief Extract a 204 request packet
             * @param userId The user id to retrieve information about
             *
             * The 204 packet is used to retrieve information about a specific contact id
             */
            void extractListContactInfoPacket(long &userId);
            /*!
             * @brief Extract a 210 request packet
             * @param userId The user id to call
             *
             * The 210 packet is used to start a call with a user
             */
            void extractStartCallPacket(long &userId);
            /*!
             * @brief Extract a 211 request packet
             * @param userId The user id to hangup with
             *
             * The 211 packet is used to end a call with a user
             */
            void extractHangupCallPacket(long &userId);
            /*!
             * @brief Extract a 212 request packet
             * @param userId The user id to accept or deny a call with
             * @param response The response to give (true or false)
             *
             * The 212 packet is used to accept or deny a call
             */
            void extractSendResponseCallPacket(long &userId, bool &response);
            /*!
             * @brief Extract a 220 request packet
             * @param userId The user id to which send the message
             * @param message The message to send
             *
             * The 220 packet is used to send a message to a user
             */
            void extractSendMessagePacket(long &userId, std::string &message);
            /*!
             * @brief Extract a 221 request packet
             * @param userId The user id with which the messages should be retrieved
             * @param startTimeStamp The start time stamp
             * @param endTimeStamp The end time stamp
             *
             * The 221 packet is used to retrieve all messages with a user between two time stamps
             */
            void extractRetrieveMessageListPacket(long &userId, long &startTimeStamp, long &endTimeStamp);
            /*!
             * @brief Extract a 240 request packet
             * @param username The username of the new user (should be unique)
             * @param birthDate The birth date of the new user
             * @param email The email of the new user
             * @param password The password of the new user
             *
             * The 240 packet is used to signup new users
             */
            void extractRegisterNewUserPacket(std::string &username, long &birthDate, std::string &email, std::string &password);
            /*!
             * @brief Extract a 241 request packet
             * @param username The username of the user who try to log in
             * @param password The password of the user who try to log in
             *
             * The 241 packet is used to authenticate yourself on the server
             */
            void extractLoginUserPacket(std::string &username, std::string &password);
            /*!
             * @brief Extract a 242 request packet
             * @param userId The user id of the user to update
             * @param username The new username
             * @param birthDate The new birth date
             * @param email The new email
             * @param password The new password
             *
             * The 242 packet is used to update a user information
             */
            void extractUpdateUserPacket(long &userId, std::string &username, long &birthDate, std::string &email, std::string &password);
            /*!
             * @brief Extract a 300 response packet
             * @param userId The user if from which the contact request comes
             *
             * The 300 packet is used by the server to indicate that someone asked you in contact
             */
            void extractRequestContactRequestPacket(long &userId);
            /*!
             * @brief Extract a 301 response packet
             * @param userId The user id that answered to the contact request
             * @param response The response of this user (true or false)
             *
             * The 301 packet is used by the server to indicate to a user that someone accepted / denied a friend request
             */
            void extractRequestUserContactResponsePacket(long &userId, bool &response);
            /*!
             * @brief Extract a 302 response packet
             * @param userId The user id that has deleted the user from it's list
             *
             * The 302 packet is used by the server to indicate to a user that someone removed him from the contact list
             */
            void extractDeletedUserContactListResponsePacket(long &userId);
            /*!
             * @brief Extract a 303 response packet
             * @param ids The ids in the contact list
             *
             * The 303 packet is used by the server to answer to a contact list request
             */
            void extractListContactListResponsePacket(std::vector<long> &ids);
            /*!
             * @brief Extract a 304 response packet
             * @param userId The id of the user this packet is about
             * @param username The username of the requested user
             * @param birthDate The birth date of the requested user
             * @param email The email of the requested user
             *
             * The 304 packet is used by the server to answer to a contact infos request
             */
            void extractListContactInfoResponsePacket(long &userId, std::string &username, long &birthDate, std::string &email);
            /*!
             * @brief Extract a 305 response packet
             * @param ids The ids in the contact request list
             *
             * The 305 packet is used by the server to answer to a contact request list request
             */
            void extractListContactRequestListResponsePacket(std::vector<long> &ids);
            /*!
             * @brief Extract a 310 response packet
             * @param userId The user id of the user who called
             * @param ipv4 The ip address of the caller
             * @param port The port of the caller
             *
             * The 310 packet is used to indicate that the user is being called by someone
             */
            void extractReceivedCallPacket(long &userId, int &ipv4, int &port);
            /*!
             * @brief Extract a 311 response packet
             * @param userId The id of the user who was called
             * @param response The response of the user who was called
             * @param ipv4 The ip address of the user who was called
             * @param port The port of the user who was called
             *
             * The 311 packet is used by the server to indicate to a caller that the user accepted / denied the call
             */
            void extractCallUserResponsePacket(long &userId, bool &response, int &ipv4, int &port);
            /*!
             * @brief create a 312 response packet
             * @param userId The id of the user who hanged up
             *
             * The 312 packet is used by the server to indicate that a remote user ended the call
             */
            void extractUserHangedUpPacket(long &userId);
            /*!
             * @brief Extract a 320 response packet
             * @param userId The id of the user who sent the message
             * @param message The message that was sent
             *
             * The 320 packet is used by the server to indicate to a user that he received a message
             */
            void extractReceivedMessagePacket(long &userId, std::string &message);
            /*!
             * @brief Extract a 321 response packet
             * @param userId The user of the messages list
             * @param messages The messages that were sent/received to/from this user
             *
             * The 321 packet is used by the server to response to a message list
             */
            void extractRetrieveMessageListResponsePacket(long &userId, std::vector<std::pair<uint64_t, std::string>> &messages);
            /*!
             * @brief Extract a 340 response packet
             * @param userId The user id who was logged/updated
             *
             * The 340 packet is used to send the user id who was logged in or updated
             */
            void extractUserAuthResponsePacket(long &userId);

        private:
            typedef size_t (BabelPacket::*extractFunc)(const void *data, size_t size);
            static const std::map<uint16_t, extractFunc> _opCodeExtractFuncMap;

        private:
            size_t buildSendBinaryResponseFromData(const void *data, size_t size);
            size_t buildAddUserContactListFromData(const void *data, size_t size);
            size_t buildDeleteUserContactListFromData(const void *data, size_t size);
            size_t buildListContactListFromData(const void *data, size_t size);
            size_t buildSendResponseContactRequestFromData(const void *data, size_t size);
            size_t buildListContactInfoFromData(const void *data, size_t size);
            size_t buildCreateListContactRequestListPacketFromData(const void *data, size_t size); //
            size_t buildStartCallFromData(const void *data, size_t size);
            size_t buildHangupCallFromData(const void *data, size_t size);
            size_t buildSendResponseCallFromData(const void *data, size_t size);
            size_t buildSendMessageFromData(const void *data, size_t size);
            size_t buildRetrieveMessageListFromData(const void *data, size_t size);
            size_t buildRegisterNewUserFromData(const void *data, size_t size);
            size_t buildLoginUserFromData(const void *data, size_t size);
            size_t buildUpdateUserFromData(const void *data, size_t size);
            size_t buildRequestContactRequestFromData(const void *data, size_t size);
            size_t buildRequestUserContactResponseFromData(const void *data, size_t size);
            size_t buildDeletedUserContactListResponseFromData(const void *data, size_t size);
            size_t buildListContactListResponseFromData(const void *data, size_t size);
            size_t buildListContactInfoResponseFromData(const void *data, size_t size);
            size_t buildListContactRequestListResponseFromData(const void *data, size_t size);
            size_t buildReceivedCallFromData(const void *data, size_t size);
            size_t buildCallUserResponseFromData(const void *data, size_t size);
            size_t buildUserHangedUpFromData(const void *data, size_t size);
            size_t buildReceivedMessageFromData(const void *data, size_t size);
            size_t buildRetrieveMessageListResponseFromData(const void *data, size_t size);
            size_t buildUserAuthResponseFromData(const void *data, size_t size);

        private:
            bool _valid; /*!< Is this packet valid */
            size_t _packetSize; /*!< Size of the packet data */
            BabelProtocolPacketData _rawData; /*!< Packet data */
        };
    }
}

#endif //BABEL_BABELPACKET_HPP
