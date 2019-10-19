/*
** EPITECH PROJECT, 2019
** babel-v1
** File description:
** testNetworkClient
*/

/* Created the 04/10/2019 at 16:36 by charlie.jeanneau@epitech.eu */

#include <criterion/criterion.h>
#include "../client/sources/network/NetworkClient.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

static void init()
{}

static void fini()
{}

Test(test_networkClient, create_config_file,
        .timeout=10,
        .description="Test the Network client",
        .init=init,
        .fini=fini
)
{
    b12software::network::NetworkClient networkClient;

    networkClient.run();

    cr_assert_eq(boost::filesystem::exists("config.json"), true);
}