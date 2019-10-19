/*
** EPITECH PROJECT, 2019
** b12software
** File description:
** CircularBuffer.cpp
*/

/* Created the 08/08/2019 at 16:30 by julian.frabel@epitech.eu */

#include <cstring>
#include "CircularBuffer.hpp"
#include "NotEnoughDataInCircularBufferException.hpp"
#include "b12software/exception/NullPointerException.hpp"

b12software::buffer::CircularBuffer::CircularBuffer(size_t size)
    : _bufferSize(size), _readIdx(0), _writeIdx(0), _buffer(nullptr), _full(false), _nbBufferedBytes(0)
{
    if (size == 0) {
        throw CircularBufferException("[CircularBuffer] A buffer can't have a size of 0", WHERE);
    }
    _buffer = new std::uint8_t[size];
}

b12software::buffer::CircularBuffer::CircularBuffer(const CircularBuffer &other)
    : _bufferSize(other._bufferSize), _readIdx(other._readIdx), _writeIdx(other._writeIdx), _buffer(nullptr), _full(other._full), _nbBufferedBytes(other._nbBufferedBytes)
{
    _buffer = new std::uint8_t[_bufferSize];
    std::memcpy(_buffer, other._buffer, _bufferSize);
}

b12software::buffer::CircularBuffer::~CircularBuffer()
{
    delete [] _buffer;
}

bool b12software::buffer::CircularBuffer::write(const void *data, size_t size)
{
    if (data == nullptr)
        throw exception::NullPointerException("[CircularBuffer] Data was found null on a write to circular buffer", WHERE);
    const std::uint8_t *ptr = static_cast<const std::uint8_t *>(data);
    for (size_t i = 0; i < size; ++i) {
        _buffer[_writeIdx] = ptr[i];
        _writeIdx = (_writeIdx + 1) % _bufferSize;
    }
    _nbBufferedBytes += size;
    bool overflow = (_nbBufferedBytes > _bufferSize);
    _full = (_nbBufferedBytes >= _bufferSize);
    if (overflow) {
        _nbBufferedBytes = _bufferSize;
        _readIdx = _writeIdx;
    }
    return overflow;
}

size_t b12software::buffer::CircularBuffer::read(void *toWrite, size_t size)
{
    if (toWrite == nullptr)
        throw exception::NullPointerException("[CircularBuffer] Area to write to was found null on a read from circular buffer", WHERE);
    if (_nbBufferedBytes < size)
        throw buffer::NotEnoughDataInCircularBufferException(
            "[CircularBuffer] Not enough data in the buffer when trying to read " + std::to_string(size) +
            " bytes from a buffer that contains " + std::to_string(_nbBufferedBytes) + " bytes.",
            WHERE);
    std::uint8_t *ptr = static_cast<std::uint8_t *>(toWrite);
    for (size_t i = 0; i < size; ++i) {
        ptr[i] = _buffer[_readIdx];
        _readIdx = (_readIdx + 1) % _bufferSize;
    }
    _nbBufferedBytes -= size;
    if (_nbBufferedBytes != 0)
        _full = false;
    return size;
}

void b12software::buffer::CircularBuffer::clear()
{
    _readIdx = 0;
    _writeIdx = 0;
    _full = false;
    _nbBufferedBytes = 0;
}

size_t b12software::buffer::CircularBuffer::getNbBufferedBytes() const
{
    return _nbBufferedBytes;
}

bool b12software::buffer::CircularBuffer::isFull() const
{
    return _full;
}

size_t b12software::buffer::CircularBuffer::read(std::string &str, const char *delim, size_t size)
{
    if (size == 0 || delim == nullptr)
        throw buffer::CircularBufferException("[CircularBuffer] Delimiter unspecified", WHERE);
    bool found = false;
    size_t msgSize = 0;
    for (size_t i = 0; i < _nbBufferedBytes && !found; ++i) {
        for (size_t j = 0; j < size && i + j < _nbBufferedBytes; ++j) {
            if (_buffer[(_readIdx + i + j) % _bufferSize] != delim[j])
                break;
            if (j + 1 == size) {
                found = true;
                msgSize = i + size;
            }
        }
    }
    if (found) {
        for (size_t i = 0; i < msgSize; ++i) {
            char byte = _buffer[_readIdx];
            str += byte;
            _readIdx = (_readIdx + 1) % _bufferSize;
            _nbBufferedBytes--;
            _full = false;
        }
        return msgSize;
    }
    return 0;
}

b12software::buffer::CircularBuffer &b12software::buffer::CircularBuffer::operator=(const b12software::buffer::CircularBuffer &rhs)
{
    if (&rhs == this)
        return *this;
    if (_bufferSize != rhs._bufferSize) {
        delete [] _buffer;
        _buffer = new std::uint8_t[_bufferSize];
    }
    std::memcpy(_buffer, rhs._buffer, _bufferSize);
    _bufferSize = rhs._bufferSize;
    _readIdx = rhs._readIdx;
    _writeIdx = rhs._writeIdx;
    _full = rhs._full;
    _nbBufferedBytes = rhs._nbBufferedBytes;
    return *this;
}

size_t b12software::buffer::CircularBuffer::getBufferSize() const
{
    return _bufferSize;
}
