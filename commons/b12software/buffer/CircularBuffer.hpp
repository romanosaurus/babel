/*
** EPITECH PROJECT, 2019
** b12software
** File description:
** CircularBuffers.hpp
*/

/* Created the 08/08/2019 at 16:30 by julian.frabel@epitech.eu */

#ifndef CPP_B12SOFTWARE_CIRCULARBUFFER_HPP
#define CPP_B12SOFTWARE_CIRCULARBUFFER_HPP

#include <cstdint>
#include <string>

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
         * @class CircularBuffer
         * @brief Class used as a buffer
         */
        class CircularBuffer {
        public:
            /*!
             * @brief ctor
             * @param size The size of this circular buffer in bytes
             */
            explicit CircularBuffer(size_t size);

            /*!
             * @brief cpy ctor
             * @param other The buffer to copy
             */
            CircularBuffer(const CircularBuffer &other);

            /*!
             * @brief dtor
             */
            ~CircularBuffer();

            /*!
             * @brief Assignment operator
             * @param rhs the object to assign as
             * @return *this
             */
            CircularBuffer &operator=(const CircularBuffer &rhs);

            /*!
             * @brief Write size bytes from data to the buffer
             * @param data the data to take bytes from
             * @param size the number of bytes to take from data
             * @return true if the buffer overflowed and wrote on himself false otherwise
             * @throw exception::NullPointerException if data is null
             */
            bool write(const void *data, size_t size);

            /*!
             * @brief Read size bytes from the buffer to the memory pointed by toWrite
             * @param toWrite The memory address to which write the data
             * @param size The amount of bytes to take from the buffer
             * @return The amount of bytes wrote to the memory address
             * @throw exception::NullPointerException if toWrite is null
             * @throw buffer::NotEnoughDataInCircularBufferException If there is less data than asked
             */
            size_t read(void *toWrite, size_t size);
            /*!
             * @brief Read bytes from the buffer to a delimiter string (included) and append it in str
             * @param str The string to appends the bytes to
             * @param delim The search delimiter
             * @param size The size of the delimiter
             * @return The amount of bytes that were appended to the string
             */
            size_t read(std::string &str, const char *delim, size_t size);

            /*!
             * @brief Clear the current buffer discarding the totality of it's content
             */
            void clear();

            /*!
             * @brief Check if a buffer is full (=the buffer will overflow if something else is written to it)
             * @return true if the buffer is full false otherwise
             */
            bool isFull() const;

            /*!
             * @brief Get the number of buffered bytes
             * @return The number of currently buffered bytes
             */
            [[nodiscard]] size_t getNbBufferedBytes() const;

            /*!
             * @brief Get the size of this buffer
             * @return The size of the buffer
             */
            [[nodiscard]] size_t getBufferSize() const;

        private:
            size_t _bufferSize; /*!< The size of the buffer */
            size_t _readIdx; /*!< The current read pointer (used internally) */
            size_t _writeIdx; /*!< The current write pointer (used internally) */
            std::uint8_t *_buffer; /*!< The buffered bytes */
            bool _full; /*!< Boolean storing if the buffer is full or not */
            size_t _nbBufferedBytes; /*!< Number of buffered bytes */
        };
    }
}

#endif //CPP_B12SOFTWARE_CIRCULARBUFFER_HPP
