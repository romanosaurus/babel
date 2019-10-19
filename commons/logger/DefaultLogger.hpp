/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** DefaultLogger.hpp
*/

/* Created the 18/09/2019 at 15:38 by julian.frabel@epitech.eu */

#ifndef BABEL_DEFAULTLOGGER_HPP
#define BABEL_DEFAULTLOGGER_HPP

#include "ILogger.hpp"

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
         * @class DefaultLogger
         * @brief A static class used to set and use a default logger
         */
        class DefaultLogger {
        private:
            /*!
             * @brief You should not be able to instantiate a DefaultLogger class
             */
            DefaultLogger() = default;
        public:
            /*!
             * @brief Set the logger to use as default
             * @param logger The logger to use
             */
            static void SetDefaultLogger(ILogger *logger);

            /*!
             * @brief Get the current logger
             * @return The current used logger
             */
            static ILogger *GetCurrentLogger();

            /*!
             * @brief Set the current log level to the current logger
             * @param level The level to set to
             *
             * If no logger is set will do nothing
             */
            static void SetLogLevel(LogLevel level);

            /*!
             * @brief Get the current log level to the current logger
             * @return The level the current log level is at
             *
             * If no logger is set will return LogLevelNone
             */
            static LogLevel GetLogLevel();

            /*!
             * @brief Log to the current logger
             * @param messageLevel The level of the message
             * @param message The message to log
             *
             * If no logger is set will do nothing
             */
            static void Log(LogLevel messageLevel, const std::string &message);

        private:
            static ILogger *currentLogger; /*!< The current logger to use */
        };
    }
}

#endif //BABEL_DEFAULTLOGGER_HPP
