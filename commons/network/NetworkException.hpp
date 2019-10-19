/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** NetworkException.hpp
*/

/* Created the 25/09/2019 at 20:52 by julian.frabel@epitech.eu */

#ifndef BABEL_NETWORKEXCEPTION_HPP
#define BABEL_NETWORKEXCEPTION_HPP

#include "b12software/exception/B12SoftwareException.hpp"

/*!
 * @namespace b12software
 * @brief Main namespace for all b12 software
 */
namespace b12software {

    /*!
     * @namespace b12software::network
     * @brief Main namespace for networking stuff
     */
    namespace network {

        /*!
         * @class NetworkException
         * @brief The root class of all network exceptions
         */
        class NetworkException : public b12software::exception::B12SoftwareException {
        public:
            /*!
             * @brief Ctor
             * @param what A message describing the exception
             * @param where A message describing the location of the exception
             */
            NetworkException(const std::string &what, const std::string &where): B12SoftwareException(what, where) {};
        };
    }
}

#endif //BABEL_NETWORKEXCEPTION_HPP
