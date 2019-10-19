/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** ConfigLoader.cpp
*/

/* Created the 06/10/2019 at 18:31 by julian.frabel@epitech.eu */

#include "ConfigLoader.hpp"
#include "exception/BabelException.hpp"

b12software::babel::ConfigLoader::ConfigLoader(const std::string &path, bool create)
    : _root(), _path(path)
{
    if (!boost::filesystem::exists(path)) {
        if (create) {
            boost::property_tree::ptree createFile;
            boost::property_tree::write_json(path, createFile);
        } else {
            throw BabelException("File " + path + " not found", WHERE);
        }
    }
    boost::property_tree::read_json(path, _root);
}

void b12software::babel::ConfigLoader::store(const std::string &key, const std::string &value)
{
    _root.put(key, value);
    boost::property_tree::write_json(_path, _root);
}

std::string b12software::babel::ConfigLoader::retrieve(const std::string &key, const std::string &defaultValue)
{
    return _root.get(key, defaultValue);
}
