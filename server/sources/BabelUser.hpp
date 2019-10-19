/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** BabelClientUser.hpp
*/

/* Created the 03/10/2019 at 07:18 by julian.frabel@epitech.eu */

#ifndef BABEL_BABELUSER_HPP
#define BABEL_BABELUSER_HPP

#include <string>
#include <vector>
#include "../commons/b12software/db/orm/B12Orm.hpp"

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
         * @class BabelUser
         * @brief A class representing a babel user
         */
        class BabelUser {
        public:
            /*!
             * @brief ctor
             */
            BabelUser();
            /*!
             * @brief dtor
             */
            ~BabelUser() = default;

            /*!
             * @brief Check if the user is logged in
             * @return true if the user is logged in false otherwise
             */
            bool isLoggedIn() const;

            /*!
             * @brief Check if a user is calling
             * @return true if this user is calling
             */
            bool isCalling() const;

            /*!
             * @brief Start calling an id
             * @param id The id to call
             */
            void startCalling(int id);

            /*!
             * @brief Get the user that this user is calling
             * @return the user thats beeing called
             */
            int getCalling() const;

            /*!
             * @brief Check if a user is in a call
             * @return true if this user is in a call
             */
            bool isInCall() const;

            /*!
             * @brief Enter in a call with a user
             * @param id The id to call
             */
            void enterCall(int id);

            /*!
             * @brief Get the user that this user is in call with
             * @return the user that's is called
             */
            int getCall() const;

            /*!
             * @brief Check if this user has a user id in his friends
             * @param id The user to check for
             * @return true id this user is a friend with false otherwise
             */
            bool isFriendWith(int id);

            /*!
             * @brief get this user id
             * @return this user id (0) if unlogged
             */
            int getId() const;

            /*!
             * @brief Signup a new user
             * @param username The username (must be unique)
             * @param email The email
             * @param password The password
             * @param birthdate The birth date
             * @return true if a new user was created false otherwise
             */
            bool signup(const std::string &username, const std::string &email, const std::string &password, long birthdate);

            /*!
             * @brief Attempt credentials
             * @param username The username
             * @param password The password
             * @return true if the user was logged in false otherwise
             */
            bool attempt(const std::string &username, const std::string &password);

            /*!
             * @brief Add new friend to list
             * @param newFriendId The requested friend id
             * @return true the friend request was sended false otherwise
             */
            bool addFriend(long newFriendId);

            /*!
             * @brief Accept or deny a friend request
             * @param friendId The friend to accept
             * @param response The response
             * @return true the friend request was accepted false otherwise
             */
            bool responseFriend(long friendId, bool response);

            /*!
             * @brief Delete friend from list
             * @param newFriendId The requested friend id
             * @return true the friend request was sended false otherwise
             */
            bool deleteFriend(long friendId);

            /*!
             * @brief List friends of this user
             * @param ids Reference to list of friends (Fill by this method)
             * @return true the friend request was sended false otherwise
             */
            bool listFriends(std::vector<long> &ids);

            /*!
             * @brief List friends requests to this user
             * @param ids Reference to list of friends requests (Fill by this method)
             * @return true if it's ok false otherwise
             */
            bool listFriendsRequests(std::vector<long> &ids);

            /*!
             * @brief Get profile for me or a friend
             * @param id Profile id
             * @return true if the profile request has succeed otherwise false
             */
            bool getProfile(long id, ORM::Entity &resEntity);

            /*!
             * @brief Add a message to db
             * @param idTo Id user who receive message
             * @param message Message sened
             * @return true if message saved else otherwise
             */
            bool sendMessage(long idTo, const std::string &message);

            /*!
             * @brief Retrieve message
             * @param idTo Id of the user conversation
             * @param timeStart timestamp to start retrieve
             * @param timeEnd timestamp to end retrieve
             * @param messages List of message retrieved (Filled by this method)
             * @return true if messages retrieved successfully else otherwise
             */
            bool retrieveMessageList(long idTo, long timeStart, long timeEnd, std::vector<std::pair<uint64_t, std::string>> &messages);

        private:
            int _userId; /*!< The database id corresponding to this user */
            int _calling; /*!< The user that this user is calling */
            int _inCallWith; /*!< The other user id who is in call with this user */
        };
    }
}

#endif //BABEL_BABELUSER_HPP
