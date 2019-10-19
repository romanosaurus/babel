/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** HostInfos.hpp
*/

/* Created the 24/09/2019 at 21:16 by julian.frabel@epitech.eu */

#ifndef BABEL_HOSTINFOS_HPP
#define BABEL_HOSTINFOS_HPP

#include <string>

/*!
 * @namespace b12software
 * @brief Main namespace for all b12 software
 */
namespace b12software {

    /*!
     * @namespace b12software::network
     * @brief Main namespace for networking stuff
     */
    namespace network {

        /*!
         * @struct HostInfos
         * @brief A structure holding host information for a datagram
         */
        struct HostInfos {
            std::string host; /*!< The host as an address or a domain */
            int port; /*!< The port for this host */
        };
    }
}
#endif //BABEL_HOSTINFOS_HPP
