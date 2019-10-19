/*
** EPITECH PROJECT, 2019
** b12software
** File description:
** NullPointerException.hpp
*/

/* Created the 08/08/2019 at 20:03 by julian.frabel@epitech.eu */

#ifndef CPP_B12SOFTWARE_NULLPOINTEREXCEPTION_HPP
#define CPP_B12SOFTWARE_NULLPOINTEREXCEPTION_HPP

#include "B12SoftwareException.hpp"

/*!
 * @namespace b12software
 * @brief Main namespace for all b12 software
 */
namespace b12software {

    /*!
     * @namespace b12software::exception
     * @brief Main namespace for generic exceptions
     */
    namespace exception {

        /*!
         * @class NullPointerException
         * @brief An exception describing that a null pointer was found where it wasn't expected
         */
        class NullPointerException : public B12SoftwareException {
        public:
            /*!
             * @brief Ctor
             * @param what A message describing the exception
             * @param where A message describing the location of the exception
             */
            NullPointerException(const std::string &what, const std::string &where): B12SoftwareException(what, where) {};
        };
    }
}

#endif //CPP_B12SOFTWARE_NULLPOINTEREXCEPTION_HPP
