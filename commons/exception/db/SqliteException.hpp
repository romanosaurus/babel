//
// Created by Brian on 24/09/2019.
//

#ifndef BABEL_SQLITEEXCEPTION_HPP
#define BABEL_SQLITEEXCEPTION_HPP

#include "exception/BabelException.hpp"

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
         * @class SqliteException
         * @brief The root of all exceptions for the sqlite wrapper
         */
        class SqliteException : public BabelException {
        public:
            /*!
             * @brief ctor
             * @param what A message describing the error
             */
            SqliteException(const std::string &what): BabelException(what, WHERE) {};
        };

        /*!
        * @class SqliteException
        * @brief Triggered when sqlite failed to connect
        */
        class SqliteFailedToConnectException : public SqliteException {
        public:
            /*!
             * @brief ctor
             */
            SqliteFailedToConnectException(): SqliteException("Failed to connect to database") {};
        };


        class SqliteFailedToExecException : public SqliteException {
        public:
            /*!
             * @brief ctor
             * @param what A message describing the error
             */
            explicit SqliteFailedToExecException(const std::string &errorMessage): SqliteException("Failed to exec a request"), _errorMessage(errorMessage) {};

            /*!
             * @brief Get sql error message
             * @return SQL error message
             */
            std::string getErrorMessage() const
            {
                return _errorMessage;
            }
        private:
            std::string _errorMessage; /*!< SQl error message */
        };
    }
}



#endif //BABEL_SQLITEEXCEPTION_HPP
