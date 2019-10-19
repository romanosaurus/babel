/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** main.cpp
*/

/* Created the 17/09/2019 at 16:49 by julian.frabel@epitech.eu */

#include "logger/StandardLogger.hpp"
#include "gui/BabelApplication.hpp"
#include "exception/BabelException.hpp"
#include "network/NetworkClient.hpp"

int main(int argc, char **argv)
{
    //b12software::babel::DefaultLogger::SetDefaultLogger(new b12software::babel::StandardLogger(b12software::babel::LogLevelDebug));

    try {
        b12software::network::NetworkClient client;
        b12software::babel::BabelApplication application(argc, argv, client);

        return application.run();
    } catch (b12software::network::NetworkException &e) {
        b12software::babel::DefaultLogger::Log(b12software::babel::LogLevelError, "[FATAL ERROR] " + std::string(e.what()));
    } catch (b12software::exception::B12SoftwareException &e) {
        b12software::babel::DefaultLogger::Log(b12software::babel::LogLevelError, "[FATAL ERROR] " + std::string(e.what()));
    } catch (...) {
        b12software::babel::DefaultLogger::Log(b12software::babel::LogLevelError, "Unknown Fatal Error");
    }

    delete b12software::babel::DefaultLogger::GetCurrentLogger();
}