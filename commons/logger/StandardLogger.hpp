/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** StandardLogger.hpp
*/

/* Created the 18/09/2019 at 14:34 by julian.frabel@epitech.eu */

#ifndef BABEL_STANDARDLOGGER_HPP
#define BABEL_STANDARDLOGGER_HPP

#include "logger/ILogger.hpp"

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
         * @class StandardLogger
         * @brief A logger that output on the standard error output
         */
        class StandardLogger : public ILogger {
        public:
            /*!
             * @brief ctor
             * @param defaultLevel The default log level of the logger
             */
            explicit StandardLogger(LogLevel defaultLevel = LogLevelError);
            /*!
             * @brief dtor
             */
            ~StandardLogger() override = default;

            void setLogLevel(LogLevel level) override;
            [[nodiscard]] LogLevel getLogLevel() const override;
            void log(LogLevel messageLevel, const std::string &message) override;

        private:
            LogLevel _currentLevel;
        };
    }
}

#endif //BABEL_STANDARDLOGGER_HPP
