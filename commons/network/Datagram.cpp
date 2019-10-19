/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** Datagram.cpp
*/

/* Created the 24/09/2019 at 20:40 by julian.frabel@epitech.eu */

#include <iostream>
#include <cstring>
#include "Datagram.hpp"

b12software::network::Datagram::Datagram()
    : _data(nullptr), _size(0), _infos({"", 0})
{

}

b12software::network::Datagram::Datagram(const b12software::network::HostInfos &destination)
    : _data(nullptr), _size(0), _infos(destination)
{

}

b12software::network::Datagram::Datagram(const void *data, size_t size)
    : _data(nullptr), _size(size), _infos({"", 0})
{
    _data = new byte[size];
    memcpy(_data, data, size);
}

b12software::network::Datagram::Datagram(const b12software::network::HostInfos &destination,
                                         const void *data,
                                         size_t size)
    : _data(nullptr), _size(size), _infos(destination)
{
    _data = new byte[size];
    memcpy(_data, data, size);
}

b12software::network::Datagram::Datagram(const b12software::network::Datagram &other)
    : _data(nullptr), _size(other._size), _infos(other._infos)
{
    if (_size != 0) {
        _data = new byte[_size];
        memcpy(_data, other._data, _size);
    }
}

b12software::network::Datagram::~Datagram()
{
    clear();
}

void b12software::network::Datagram::setData(const void *data, size_t size)
{
    delete [] _data;
    if (data != nullptr && size > 0) {
        _data = new byte[size];
        _size = size;
        memcpy(_data, data, _size);
    } else {
        _data = nullptr;
        _size = 0;
    }
}

void b12software::network::Datagram::setDestination(const b12software::network::HostInfos &infos)
{
    _infos = infos;
}

const void *b12software::network::Datagram::getData() const
{
    return _data;
}

size_t b12software::network::Datagram::getDatagramSize() const
{
    return _size;
}

const b12software::network::HostInfos &b12software::network::Datagram::getHostInfos() const
{
    return _infos;
}

bool b12software::network::Datagram::isValid() const
{
    return _data != nullptr && _size != 0 && !_infos.host.empty();
}

void b12software::network::Datagram::clear()
{
    delete [] _data;
    _data = nullptr;
    _size = 0;
    _infos = {"", 0};
}

b12software::network::Datagram &b12software::network::Datagram::operator=(const b12software::network::Datagram &rhs)
{
    if (&rhs == this)
        return *this;
    delete _data;
    _size = rhs._size;
    _infos = rhs._infos;
    if (_size != 0) {
        _data = new byte[_size];
        memcpy(_data, rhs._data, _size);
    } else {
        _data = nullptr;
    }
    return *this;
}
