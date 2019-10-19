//
// Created by Brian on 26/09/2019.
//

#include <ctime>
#include "logger/DefaultLogger.hpp"
#include "b12software/db/orm/ORMContainer.hpp"
#include "Api.hpp"

void b12software::babel::API::Signup(const std::string &username, const std::string &password,
                                     const std::string &email, long dateBorn, std::function<void(bool)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false);

    orm->find("users", [callback, username, password, email, dateBorn](ORM::Entity entity) {
        ORM *orm2 = ORMContainer::GetCurrentORM();

        if (orm2 == nullptr)
            return callback(false);

        if (std::stoi(entity["count"]) > 0)
            return callback(false);

        orm2->create("users", {
            {"username", username},
            {"password", password},
            {"email", email},
            {"date_born", std::to_string(dateBorn)},
        });

        return callback(true);
    }, {{{"username", "='" + username + "'"}}, 1}, {"COUNT(*) AS count"});
}

void b12software::babel::API::Signin(const std::string &username, const std::string &password,
                                     std::function<void(bool, int)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false, -1);

    orm->find("users", [callback](ORM::Entity entity){
        if (std::stoi(entity["count"]) > 0)
            return callback(true, std::stoi(entity["id"]));
        return callback(false, -1);
    }, {{{"username", "='" + username + "'"}, {"password", "='" + password + "'"}}, 1}, {"COUNT(*) AS count", "id"});
}

void b12software::babel::API::UpdateProfile(int id, const std::string &newUsername, const std::string &newPassword,
                                            std::function<void(bool)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false);

    orm->find("users", [callback, id, newUsername, newPassword](ORM::Entity entity) {
        ORM *orm2 = ORMContainer::GetCurrentORM();

        if (orm2 == nullptr)
            return callback(false);

        if (std::stoi(entity["count"]) > 0)
            return callback(false);

        orm2->updateById("users", id, {{"username", newUsername}, {"password", newPassword}});

        return callback(true);
    }, {{{"id", "='" + std::to_string(id) + "'"}}, 1}, {"COUNT(*) AS count"});
}

void b12software::babel::API::GetProfile(int id, std::function<void(bool, const ORM::Entity &)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false, ORM::Entity());

    orm->findById("users", id, [callback](ORM::Entity entity) {
        return callback(true, entity);
    }, {"username", "email", "date_born"});
}

void
b12software::babel::API::SearchUsers(const std::string &searchField, std::function<void(const ORM::Entity &)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(ORM::Entity());
    orm->find("users", [callback](ORM::Entity entity){
        callback(entity);
    }, {{{"username", " LIKE '" + searchField + "%'"}}});
}

void b12software::babel::API::AddFriend(int userId, int friendId, std::function<void(bool)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false);
    orm->find("users", [userId, friendId, callback](ORM::Entity entity) {
        ORM *orm2 = ORMContainer::GetCurrentORM();

        if (entity["count"] != "2")
            return callback(false);

        if (orm2 == nullptr)
            return callback(false);

        orm2->find("friends_requests", [userId, friendId, callback](ORM::Entity entity2) {
            ORM *orm3 = ORMContainer::GetCurrentORM();

            if (std::stoi(entity2["count"]) > 0)
                return callback(false);
            if (orm3 == nullptr)
                return callback(false);

            orm3->create("friends_requests", {{"from_id", std::to_string(userId)}, {"to_id", std::to_string(friendId)}});
        }, {{{"from_id", " = " + std::to_string(userId)}, {"to_id", " = " + std::to_string(friendId)}}}, {"COUNT(*) AS count", "from_id", "to_id"});
        callback(true);
    }, {{{"id", " = " + std::to_string(userId) + " OR id = " + std::to_string(friendId)}}}, {"COUNT(*) AS count"});
}

void b12software::babel::API::ListFriend(int userId, std::function<void(bool, const ORM::Entity &)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false, ORM::Entity());

    orm->getSQLConnection().exec("SELECT * FROM friends INNER JOIN users ON friends.second_id = users.id WHERE first_id=" + std::to_string(userId), [callback](ORM::Entity entity) {
        return callback(true, entity);
    });
}

void b12software::babel::API::ListFriendRequests(int userId, std::function<void(bool, const ORM::Entity &)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false, ORM::Entity());
    orm->find("friends_requests", [callback](ORM::Entity entity) {
        callback(true, entity);
    }, {{{"to_id", "=" + std::to_string(userId)}}});
}

void b12software::babel::API::RemoveFriend(int userId, int friendId, std::function<void(bool)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false);

    std::string whereClause = " WHERE (first_id = " + std::to_string(userId) + " AND second_id = " + std::to_string(friendId) + ") OR ";
    whereClause += "(first_id = " + std::to_string(friendId) + " AND second_id = " + std::to_string(userId) + ")";
    orm->remove("friends", SQLite::sqliteCallback(), {{}, -1, -1, "",  whereClause});
    callback(true);
}

void
b12software::babel::API::AcceptOrRefuseFriend(int userId, int requesterId, bool accept, std::function<void(bool)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false);
    orm->find("users", [userId, requesterId, callback, accept](ORM::Entity entity) {
        ORM *orm2 = ORMContainer::GetCurrentORM();

        if (orm2 == nullptr || entity["count"] != "2")
            return callback(false);
        orm2->find("friends_requests", [callback, accept](ORM::Entity requestEntity) {
            ORM *orm3 = ORMContainer::GetCurrentORM();

            if (orm3 == nullptr)
                return callback(false);
            if (std::stoi(requestEntity["count"]) < 1)
                return callback(false);

            orm3->remove("friends_requests", SQLite::sqliteCallback(),  {{{"from_id", " = " + requestEntity["from_id"]}, {"to_id", " = " + requestEntity["to_id"]}}});
            if (accept) {
                orm3->create("friends", {{"first_id",  requestEntity["from_id"]}, {"second_id", requestEntity["to_id"]}});
                orm3->create("friends", {{"second_id", requestEntity["from_id"]}, {"first_id",  requestEntity["to_id"]}});
            }

            callback(true);
        }, {{{"from_id", " = " + std::to_string(requesterId)}, {"to_id", " = " + std::to_string(userId)}}}, {"COUNT(*) AS count", "from_id", "to_id"});
    }, {{{"id", " = " + std::to_string(userId) + " OR id = " + std::to_string(requesterId)}}}, {"COUNT(*) AS count"});
}

void b12software::babel::API::AddMessage(int idFrom, int idTo, const std::string &message,
                                         std::function<void(bool)> callback)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false);
    orm->find("users", [callback, idFrom, idTo, message](ORM::Entity entity) {
        ORM *orm2 = ORMContainer::GetCurrentORM();

        if (orm2 == nullptr || entity["count"] != "2")
            return callback(false);
        orm2->create("messages", {
            {"from_id", std::to_string(idFrom)},
            {"to_id", std::to_string(idTo)},
            {"time_created", std::to_string(std::time(nullptr))},
            {"message", message}
        });
        return callback(true);
    }, {{{"id", " = " + std::to_string(idFrom) + " OR id = " + std::to_string(idTo)}}}, {"COUNT(*) AS count"});
}

void b12software::babel::API::GetMessages(int idFrom, int idTo, long timestampStart, long timestampEnd,
                                          std::function<void(bool, const ORM::Entity &)> callback,
                                          int limitTo)
{
    ORM *orm = ORMContainer::GetCurrentORM();

    if (orm == nullptr)
        return callback(false, ORM::Entity());
    orm->find("messages", [callback](ORM::Entity entity) {
        callback(true, entity);
    }, {{{"(from_id", " = " + std::to_string(idFrom) + " OR from_id = " + std::to_string(idTo) + ")"}, {"(to_id", " = " + std::to_string(idTo) + " OR to_id = " + std::to_string(idFrom) + ")"},
         {"time_created", " >= " + std::to_string(timestampStart) + " AND time_created <= " + std::to_string(timestampEnd)}}, limitTo, 0, " ORDER BY time_created"});
}


