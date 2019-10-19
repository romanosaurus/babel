/*
** EPITECH PROJECT, 2019
** b12software
** File description:
** B12SoftwareException.hpp
*/

/* Created the 08/08/2019 at 17:09 by julian.frabel@epitech.eu */

#ifndef CPP_B12SOFTWARE_B12SOFTWAREEXCEPTION_HPP
#define CPP_B12SOFTWARE_B12SOFTWAREEXCEPTION_HPP

#include <exception>
#include <string>

#define WHERE (std::string(__FILE__) + " " + std::to_string(__LINE__))

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
         * @class B12SoftwareException
         * @brief A class representing a generic exception of the b12software namespace
         */
        class B12SoftwareException : public std::exception {
        public:
            /*!
             * @brief Ctor
             * @param what A message describing the exception
             * @param where A message describing the location of the exception
             */
            B12SoftwareException(const std::string &what, const std::string &where): _what(what), _where(where) {};
            /*!
             * @brief dtor
             */
            ~B12SoftwareException() override = default;

            /*!
             * @brief Get the message describing the exception
             * @return A message describing the exception
             */
            const char *what() const noexcept override { return _what.c_str(); };
            /*!
             * @brief Get a message describing the location of the exception
             * @return A message describing the location of the exception
             */
            const char *where() const noexcept { return _where.c_str(); };

        private:
            std::string _what; /*!< A message describing the exception */
            std::string _where; /*!< A message describing the location of the exception */
        };
    }
}

#endif //CPP_B12SOFTWARE_B12SOFTWAREEXCEPTION_HPP
