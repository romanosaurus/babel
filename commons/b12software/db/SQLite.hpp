//
// Created by Brian on 24/09/2019.
//

#ifndef BABEL_SQLITE_HPP
#define BABEL_SQLITE_HPP

#include <sqlite3.h>
#include <string>
#include <map>
#include <functional>

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
         * @class SQLite
         * @brief A sqlite3 wrapper
         */
        class SQLite {
        public:

            typedef std::function<void(std::map<std::string, std::string>)> sqliteCallback;

            /*!
             * @brief ctor
             * @param dbName
             */
            SQLite(const std::string &dbName);

            /*!
             * @brief Open db connection
             * @param dbName
             * @return
             */
            void connect();

            /*!
             * @brief Close db connection
             */
            void disconnect();

            /*!
             * @brief Execute a sql request
             * @param request
             * @param callback
             * @param data
             */
            void exec(const std::string &request, sqliteCallback dataReceived = sqliteCallback()) const;

        private:
            std::string _dbName; /*!< Name of the database */
            sqlite3 *_db;/*!< SQLlite Database object */
        };
    }
};

#endif //BABEL_SQLITE_HPP
