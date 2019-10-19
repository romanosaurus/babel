//
// Created by Brian on 03/10/2019.
//

#ifndef BABEL_BABELSERVER_HPP
#define BABEL_BABELSERVER_HPP

#include <map>
#include "network/ITcpServer.hpp"
#include "network/asio/AsioNetworkManager.hpp"
#include "protocol/BabelPacket.hpp"
#include "BabelUser.hpp"

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
         * @class BabelServer
         * @brief Main babel server class
         */
        class BabelServer {
        public:
            /*!
             * @brief ctor
             */
            explicit BabelServer();

            /*!
             * @brief dtor
             */
            ~BabelServer() = default;

            /*!
             * @brief Bind a port to the server
             * @param port
             */
            void run(int port);

            /*!
             * @brief stop the server
             */
            void stop();

        private:

            /*!
             * @brief Get client id by user id
             * @param userId App user id to find
             * @return id if user is find 0 otherwise
             */
            int getClientIdByUser(int userId);

            /*!
             * @brief receive data from all clients
             */
            void receive();

            /*!
             * @brief handle connection to the server
             */
            void handleConnection();

            /*!
             * @brief handle disconnection from the server
             */
            void handleDisconnection();

            /*!
             * @brief handle a packet that was received by a network client
             * @param clientId The id of the network client that sent this packet
             * @param packet The packet that was received
             */
            void handlePacket(int clientId, BabelPacket &packet);

            /*!
             * @brief handle a add friend request
             * @param clientId The id of the network client that sent this packet
             * @param packet The packet that was received
             */
            void handleAddFriendRequest(int clientId, BabelPacket &packet);

            /*!
            * @brief handle a delete friend request
            * @param clientId The id of the network client that sent this packet
            * @param packet The packet that was received
            */
            void handleDeleteFriendRequest(int clientId, BabelPacket &packet);

            /*!
            * @brief handle a friend list request
            * @param clientId The id of the network client that sent this packet
            * @param packet The packet that was received
            */
            void handleFriendListRequest(int clientId, BabelPacket &packet);

            /*!
             * @brief handle a friend request response
            * @param clientId The id of the network client that sent this packet
            * @param packet The packet that was received
             */
            void handleFriendRequestResponse(int clientId, BabelPacket &packet);

            /*!
             * @brief handle friends request list
             * @param clientId
             * @param packet
             */
            void handleListFriendsRequest(int clientId, BabelPacket &packet);

            /*!
             * @brief handle a profile request
             * @param clientId The id of the network client that sent this packet
             * @param packet The packet that was received
             */
            void handleProfileRequest(int clientId, BabelPacket &packet);

            /*!
             * @brief handle a start call request
             * @param callerId The id of the network client that sent this packet
             * @param packet The packet that was received
             */
            void handleStartCallRequest(int callerId, BabelPacket &packet);

            /*!
             * @brief handle a call request response
             * @param clientId The id of the network client that sent this packet
             * @param packet The packet that was received
             */
            void handleCallResponse(int clientId, BabelPacket &packet);

            /*!
             * @brief handle a call hangup
             * @param clientId The id of the network client that sent this packet
             * @param packet The packet that was received
             */
            void handleHangUp(int clientId, BabelPacket &packet);

            /*!
             * @brief handle a signup request
             * @param clientId The id of the network client that sent this packet
             * @param packet The packet that was received
             */
            void handleSignupRequest(int clientId, BabelPacket &packet);

            /*!
             * @brief handle a signin request
             * @param clientId The id of the network client that sent this packet
             * @param packet The packet that was received
             */
            void handleSigninRequest(int clientId, BabelPacket &packet);

            /*!
             * @brief handle send message request
             * @param clientId The id of the network client that sent this packet
             * @param packet The packet that was received
             */
            void handleSendMessages(int clientId, BabelPacket &packet);

            /*!
            * @brief handle retrieve message list
            * @param clientId The id of the network client that sent this packet
            * @param packet The packet that was received
            */
            void handleRetrieveMessageList(int clientId, BabelPacket &packet);

        private:
            /*!
             * @brief Handle a signal in the babel server
             */
            static void SignalHandler(int signal);

            static bool caughtSigInt; /*!< static variable that check if a ctrl+c was caught */

        private:
            typedef void (BabelServer::*handleFunc)(int clientId, BabelPacket &packet);
            static const std::map<uint16_t, handleFunc> opCodeHandleMap;

        private:
            std::unique_ptr<network::INetworkManager> _networkManager; /*!< The network manager of the babel server */
            network::ITcpServer *_server; /*!< The network server of the babel server */
            bool _isRunning; /*!< A boolean checking if the server is running or not */
            std::map<int, BabelUser> _clientUserMap; /*!< A map linking a network client to a babel user */
        };
    }
};


#endif //BABEL_BABELSERVER_HPP
