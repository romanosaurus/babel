/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** main.cpp
*/

/* Created the 17/09/2019 at 16:50 by julian.frabel@epitech.eu */

#include <iostream>
#include <string>
#include <b12software/db/orm/ORMContainer.hpp>
#include <b12software/exception/B12SoftwareException.hpp>
#include "BabelServer.hpp"
#include "logger/DefaultLogger.hpp"
#include "logger/StandardLogger.hpp"
#include "ConfigLoader.hpp"

using DefaultLogger = b12software::babel::DefaultLogger;

int main([[maybe_unused]]int argc, [[maybe_unused]]char **argv)
{
    DefaultLogger::SetDefaultLogger(new b12software::babel::StandardLogger(b12software::babel::LogLevelDebug));

    try {
        b12software::babel::ConfigLoader config("server_config.json");
        std::string dbName = config.retrieve("dbName", "babel.db");
        std::string port = config.retrieve("port", "4242");
        config.store("dbName", dbName);
        config.store("port", port);

        DefaultLogger::Log(b12software::babel::LogLevelInfo, "Starting server on port " + port + " using database " + dbName);
        b12software::babel::ORMContainer::SetDefaultORM(dbName);
        b12software::babel::BabelServer server;
        server.run(std::stoi(port));
    } catch (b12software::exception::B12SoftwareException &e) {
        std::cerr << "FATAL ERROR: " << e.what() << " in " << e.where() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "UNKNOWN FATAL ERROR" << std::endl;
    }

    b12software::babel::ORMContainer::CleanORM();

    delete DefaultLogger::GetCurrentLogger();
    DefaultLogger::SetDefaultLogger(nullptr);
    return 0;
}