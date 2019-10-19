//
// Created by Brian on 24/09/2019.
//

#include <iostream>
#include "logger/DefaultLogger.hpp"
#include "exception/db/SqliteException.hpp"
#include "SQLite.hpp"

b12software::babel::SQLite::SQLite(const std::string &dbName)
    :   _dbName(dbName),
        _db(nullptr)
{
}

void b12software::babel::SQLite::connect()
{
    int res = sqlite3_open(_dbName.c_str(), &_db);

    if (res)
        throw SqliteFailedToConnectException();
}

void b12software::babel::SQLite::disconnect()
{
    sqlite3_close(_db);
}

void
b12software::babel::SQLite::exec(const std::string &request, b12software::babel::SQLite::sqliteCallback dataReceived) const
{
    char *errMessage = nullptr;

    DefaultLogger::Log(LogLevelDebug, "[SQLite] " + request);
    int res = sqlite3_exec(_db, request.c_str(), [](void *data, int ac, char **av, char **columnName) {
        std::map<std::string, std::string> result;
        sqliteCallback callback = *((sqliteCallback *) data);

        for (int i = 0; i < ac; i++) {
            if (columnName[i] == nullptr || av[i] == nullptr)
                continue;
            result[std::string(columnName[i])] = std::string(av[i]);
        }
        callback(result);
        return 0;
    }, &dataReceived, &errMessage);
    if (res)
        throw SqliteFailedToExecException(std::string(errMessage));
}
