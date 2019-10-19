/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** ILogger.hpp
*/

/* Created the 18/09/2019 at 14:19 by julian.frabel@epitech.eu */

#ifndef BABEL_ILOGGER_HPP
#define BABEL_ILOGGER_HPP

#include <string>

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
         * @enum LogLevel
         * @brief An enum describing the level of the log message
         */
        enum LogLevel {
            LogLevelDebug = 0, /*!< A debug information used in development */
            LogLevelInfo = 10, /*!< Information that does not harm in anyway */
            LogLevelWarn = 20, /*!< Information that can harm the program under certain condition */
            LogLevelError = 30, /*!< An error that occurred */
            LogLevelFatal = 40, /*!< A fatal error that can lead to program end */
            LogLevelNone = 100, /*!< No logging */
        };

        /*!
         * @class ILogger
         * @brief Interface used for all loggers
         */
        class ILogger {
        public:
            /*!
             * @brief dtor
             */
            virtual ~ILogger() = default;

            /*!
             * @brief Set the log level of this logger
             * @param level The level to be set to
             *
             * Log messages that will have a lower priority than the log level will be ignored
             */
            virtual void setLogLevel(LogLevel level) = 0;

            /*!
             * @brief Get the current log level of this logger
             * @return The current log level
             */
            [[nodiscard]] virtual LogLevel getLogLevel() const = 0;

            /*!
             * @brief Log a message in the logger
             * @param messageLevel The message level
             * @param message The message
             *
             * If the message level is lower that the log level of the message it should be ignored
             */
            virtual void log(LogLevel messageLevel, const std::string &message) = 0;
        };
    }
}

#endif //BABEL_ILOGGER_HPP
