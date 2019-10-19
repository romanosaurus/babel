/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** ThreadSafeCircularBuffer.hpp
*/

/* Created the 26/09/2019 at 20:51 by julian.frabel@epitech.eu */

#ifndef BABEL_THREADSAFECIRCULARBUFFER_HPP
#define BABEL_THREADSAFECIRCULARBUFFER_HPP

#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/circular_buffer.hpp>

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
         * @brief A thread safe circular buffer based on boost one
         * @tparam T The type buffered
         */
        template<typename T>
        class ThreadSafeCircularBuffer {
        private:
            using mutex = boost::recursive_mutex;
            using lock = mutex::scoped_lock;

        public:
            /*!
             * @brief ctor
             * @param size The size of the buffer to create (in number of elements)
             */
            explicit ThreadSafeCircularBuffer(size_t size): _mutex(), _buffer(size), _nbBufferedElem(0), _bufferSize(size)
            {

            }

            /*!
             * @brief Write data to the buffer
             * @param data The data to write (should be at least nbElem size)
             * @param nbElem The number of element to write
             * @return true if the buffer overflowed false otherwise
             */
            bool write(const T *data, size_t nbElem)
            {
                lock lk(_mutex);
                for (size_t i = 0; i < nbElem; ++i) {
                    _buffer.push_back(data[i]);
                    _nbBufferedElem++;
                }
                if (_nbBufferedElem > _bufferSize) {
                    _nbBufferedElem = _bufferSize;
                    return true;
                }
                return false;
            }

            /*!
             * @brief Read elements from the buffer
             * @param buffer The buffer to write elements to
             * @param nbElem The number of elements to read
             * @return The number of element read
             */
            size_t read(T *buffer, size_t nbElem)
            {
                lock lk(_mutex);
                size_t i = 0;
                for (i = 0; i < nbElem && _nbBufferedElem != 0; ++i) {
                    buffer[i] = _buffer.front();
                    _buffer.pop_front();
                    _nbBufferedElem--;
                }
                return i;
            }

            /*!
             * @brief clear the buffer
             */
            void clear()
            {
                lock lk(_mutex);
                _nbBufferedElem = 0;
                _buffer.clear();
            }

            /*!
             * @brief Check if the buffer is full
             * @return true if the buffer is full false otherwise
             */
            bool isFull()
            {
                lock lk(_mutex);
                return _nbBufferedElem == _bufferSize;
            }

            /*!
             * @brief Get the number of buffered elements
             * @return The number of buffered elements
             */
            size_t getNbBufferedElements() const
            {
                return _nbBufferedElem;
            }

            /*!
             * @brief Lock this circular buffer
             *
             * DO NOT FORGET TO CALL unlockBuffer() OR YOU WILL CREATE DEADLOCK
             */
            void lockBuffer()
            {
                _mutex.lock();
            }

            /*!
             * @brief Unlock this circular buffer
             */
            void unlockBuffer()
            {
                _mutex.unlock();
            }

        private:
            mutex _mutex; /*!< This buffer mutex */
            boost::circular_buffer<T> _buffer; /*!< The internal boost circular buffer */
            size_t _nbBufferedElem; /*!< Number of buffered elements */
            size_t _bufferSize; /*!< Maximum number of elements that can be buffered */
        };
    }
}

#endif //BABEL_THREADSAFECIRCULARBUFFER_HPP
