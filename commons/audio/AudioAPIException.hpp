/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** AudioAPIException.hpp
*/

/* Created the 17/09/2019 at 18:26 by julian.frabel@epitech.eu */

#ifndef BABEL_AUDIOAPIEXCEPTION_HPP
#define BABEL_AUDIOAPIEXCEPTION_HPP

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
         * @class AudioAPIException
         * @brief Root exception of all AudioAPI exceptions
         */
        class AudioAPIException : public BabelException {
        public:
            /*!
             * @brief ctor
             * @param what A message describing the error
             * @param where A message describing the location
             */
            AudioAPIException(const std::string &what, const std::string &where): BabelException(what, where) {};
        };
    }
}

#endif //BABEL_AUDIOAPIEXCEPTION_HPP
