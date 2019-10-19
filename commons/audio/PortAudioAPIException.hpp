/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** PortAudioAPIException.hpp
*/

/* Created the 18/09/2019 at 11:57 by julian.frabel@epitech.eu */

#ifndef BABEL_PORTAUDIOAPIEXCEPTION_HPP
#define BABEL_PORTAUDIOAPIEXCEPTION_HPP

#include "audio/AudioAPIException.hpp"

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
         * @class PortAudioAPIException
         * @brief Root exception of all PortAudioAPI exceptions
         */
        class PortAudioAPIException : public AudioAPIException {
        public:
            /*!
             * @brief ctor
             * @param what A message describing the error
             * @param where A message describing the location
             */
            PortAudioAPIException(const std::string &what, const std::string &where): AudioAPIException(what, where) {};
        };
    }
}

#endif //BABEL_PORTAUDIOAPIEXCEPTION_HPP
