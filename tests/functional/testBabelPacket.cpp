/*
** EPITECH PROJECT, 2019
** babel-v1
** File description:
** testBabelPacket
*/

/* Created the 04/10/2019 at 18:04 by charlie.jeanneau@epitech.eu */

#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(test_babelPacket, get_set,
        .timeout=10,
        .description="Test the BabelPacket",
        .init=init,
        .fini=fini
)
{
    b12software::babel::BabelPacket packet;

    cr_assert_eq(packet.isValid(), false);
    cr_assert_eq(packet.getSize(), 0);
}