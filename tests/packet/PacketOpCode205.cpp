/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode205
*/

/* Created the 03/10/2019 at 13:56 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_205,
        .timeout=10,
        .description="Text the serialization/deserialization of a opcode 205",
        .init=init,
        .fini=fini
)
{
    b12software::babel::BabelPacket packet;

    packet.createListContactRequestListPacket();

    cr_assert_eq(packet.getPacketOpCode(), 205, "Expected packet op code to be %u but got %u", 205, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 2, "Expected packet op size to be %lu, but got %lu", 2, packet.getSize());
}