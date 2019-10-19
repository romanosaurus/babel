/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode210
*/

/* Created the 03/10/2019 at 14:22 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_210,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 210",
        .init=init,
        .fini=fini
)
{
    long serializeId = 740;
    long deserializedId;

    b12software::babel::BabelPacket packet;

    packet.createStartCallPacket(serializeId);
    packet.extractStartCallPacket(deserializedId);

    cr_assert_eq(packet.getPacketOpCode(), 210, "Expected packet op code to be %u but got %u", 210, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 10, "Expected packet op size to be %lu, but got %lu", 10, packet.getSize());
    cr_assert_eq(deserializedId, serializeId, "Expected deserializedResponse to be %u, but go %u", deserializedId, serializeId);
}