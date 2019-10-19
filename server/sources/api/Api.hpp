//
// Created by Brian on 26/09/2019.
//

#ifndef BABEL_API_HPP
#define BABEL_API_HPP

#include <string>
#include "b12software/db/orm/B12Orm.hpp"

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
         * @class API
         * @brief Class used as a buffer
         */
        class API {
        public:

            /*!
             * @brief Create a new user
             * @param username
             * @param password
             * @param callback
             */
            static void Signup(const std::string &username, const std::string &password, const std::string &email, long dateBorn, std::function<void(bool)> callback);

            /*!
             * @brief Login to a user
             * @param username
             * @param password
             * @param callback
             */
            static void Signin(const std::string &username, const std::string &password, std::function<void(bool, int)> callback);

            /*!
             * @brief Update a user profile
             * @param id
             * @param newUsername
             * @param newPassword
             * @param callback
             */
            static void UpdateProfile(int id, const std::string &newUsername, const std::string &newPassword, std::function<void(bool)> callback);

            /*!
             * @brief get a user profile
             * @param id
             * @param callback
             */
            static void GetProfile(int id, std::function<void(bool success, const ORM::Entity &)> callback);

            /*!
             * @brief search for user
             * @param searchField
             * @param callback
             */
            static void SearchUsers(const std::string &searchField, std::function<void(const ORM::Entity &)> callback);

            /*!
             * @brief Add a user as friend
             * @param userId
             * @param friendId
             * @param callback
             */
            static void AddFriend(int userId, int friendId, std::function<void(bool)> callback);

            /*!
             * @brief accept a friend request
             * @param usedId
             * @param friendId
             * @param callback
             */
            static void AcceptOrRefuseFriend(int userId, int friendId, bool accept, std::function<void(bool)> callback);

            /*!
             * @brief List friends of a user
             * @param userId
             * @param callback
             */
            static void ListFriend(int userId, std::function<void(bool, const ORM::Entity &)> callback);

            /*!
             * @brief List friends requests of a user
             * @param userId
             * @param callback
             */
            static void ListFriendRequests(int userId, std::function<void(bool, const ORM::Entity &)> callback);

            /*!
             * @brief remove a friend from list
             * @param userId
             * @param friendId
             * @param callback
             */
            static void RemoveFriend(int userId, int friendId, std::function<void(bool)> callback);

            /*!
             * @brief add a message
             * @param idFrom
             * @param idTo
             * @param dateCreated
             * @param message
             * @param callback
             */
            static void AddMessage(int idFrom, int idTo, const std::string &message, std::function<void(bool)> callback);

            /*!
             * @brief Get messages
             * @param idFrom
             * @param idTo
             * @param timestampStart
             * @param timestampEnd
             * @param callback
             */
            static void GetMessages(int idFrom, int idTo, long timestampStart, long timestampEnd, std::function<void(bool, const ORM::Entity &entity)> callback, int limitTo = 100);
        };
    }
};

#endif //BABEL_API_HPP
