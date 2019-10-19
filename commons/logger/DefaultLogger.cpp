/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** DefaultLogger.cpp
*/

/* Created the 18/09/2019 at 15:38 by julian.frabel@epitech.eu */

#include "DefaultLogger.hpp"

b12software::babel::ILogger *b12software::babel::DefaultLogger::currentLogger = nullptr;

void b12software::babel::DefaultLogger::SetDefaultLogger(b12software::babel::ILogger *logger)
{
    DefaultLogger::currentLogger = logger;
}

b12software::babel::ILogger *b12software::babel::DefaultLogger::GetCurrentLogger()
{
    return DefaultLogger::currentLogger;
}

void b12software::babel::DefaultLogger::SetLogLevel(b12software::babel::LogLevel level)
{
    if (DefaultLogger::currentLogger != nullptr) {
        DefaultLogger::currentLogger->setLogLevel(level);
    }
}

b12software::babel::LogLevel b12software::babel::DefaultLogger::GetLogLevel()
{
    if (DefaultLogger::currentLogger != nullptr) {
        return DefaultLogger::currentLogger->getLogLevel();
    }
    return LogLevelNone;
}

void b12software::babel::DefaultLogger::Log(b12software::babel::LogLevel messageLevel, const std::string &message)
{
    if (DefaultLogger::currentLogger != nullptr) {
        DefaultLogger::currentLogger->log(messageLevel, message);
    }
}
