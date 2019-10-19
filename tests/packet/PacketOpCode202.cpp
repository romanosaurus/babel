/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode202
*/

/* Created the 03/10/2019 at 13:56 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_202,
        .timeout=10,
        .description="Text the serialization/deserialization of a opcode 202",
        .init=init,
        .fini=fini
)
{
    b12software::babel::BabelPacket packet;

    packet.createListContactListPacket();

    cr_assert_eq(packet.getPacketOpCode(), 202, "Expected packet op code to be %u but got %u", 202, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 2, "Expected packet op size to be %lu, but got %lu", 2, packet.getSize());
}