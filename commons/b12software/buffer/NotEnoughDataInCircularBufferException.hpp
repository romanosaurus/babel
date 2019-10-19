/*
** EPITECH PROJECT, 2019
** b12software
** File description:
** NotEnoughDataInCircularBufferException.hpp
*/

/* Created the 08/08/2019 at 19:22 by julian.frabel@epitech.eu */

#ifndef CPP_B12SOFTWARE_NOTENOUGHDATAINCIRCULARBUFFEREXCEPTION_HPP
#define CPP_B12SOFTWARE_NOTENOUGHDATAINCIRCULARBUFFEREXCEPTION_HPP

#include "CircularBufferException.hpp"

/*!
 * @namespace b12software
 * @brief Main namespace for all b12 software
 */
namespace b12software {

    /*!
     * @namespace b12software::buffer
     * @brief Main namespace for custom buffers
     */
    namespace buffer {

        /*!
         * @brief A class describing a try to read an amount of bytes from a buffer that does not have the needed amount of bytes
         */
        class NotEnoughDataInCircularBufferException : public CircularBufferException {
        public:
            /*!
             * @brief Ctor
             * @param what A message describing the exception
             * @param where A message describing the location of the exception
             */
            NotEnoughDataInCircularBufferException(const std::string &what, const std::string &where): CircularBufferException(what, where) {};
        };
    }
}

#endif //CPP_B12SOFTWARE_NOTENOUGHDATAINCIRCULARBUFFEREXCEPTION_HPP
