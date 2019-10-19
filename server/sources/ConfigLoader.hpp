/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** ConfigLoader.hpp
*/

/* Created the 06/10/2019 at 18:31 by julian.frabel@epitech.eu */

#ifndef BABEL_CONFIGLOADER_HPP
#define BABEL_CONFIGLOADER_HPP

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

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
         * @class ConfigLoader
         * @brief A class used to load a config
         */
        class ConfigLoader {
        public:
            /*!
             * @brief Load a config in a given path
             * @param path The path to load
             * @param create Should the file be created if not found
             */
            explicit ConfigLoader(const std::string &path, bool create = true);

        public:
            /*!
             * @brief Store a configuration value
             * @param key The key to store
             * @param value The value to store
             */
            void store(const std::string &key, const std::string &value);
            /*!
             * @brief Get a configuration value
             * @param key The key to search for
             * @param defaultValue The value to return if the key isn't found
             * @return The value found
             */
            std::string retrieve(const std::string &key, const std::string &defaultValue);

        private:
            boost::property_tree::ptree _root; /*!< Boost root node */
            std::string _path; /*!< The config file path */
        };
    }
}

#endif //BABEL_CONFIGLOADER_HPP
