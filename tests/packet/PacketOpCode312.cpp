/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode312
*/

/* Created  the 03/10/2019 at 18:46 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_312,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 312",
        .init=init,
        .fini=fini
)
{
    long serializeId = 293;
    long deserialisedId;

    b12software::babel::BabelPacket packet;

    packet.createUserHangedUpPacket(serializeId);
    packet.extractUserHangedUpPacket(deserialisedId);

    cr_assert_eq(packet.getPacketOpCode(), 312, "Expected packet op code to be %u but got %u", 312, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 10, "Expected packet op size to be %lu, but got %lu", 10, packet.getSize());
    cr_assert_eq(deserialisedId, serializeId, "Expected deserializedId to be %u, but go %u", deserialisedId, serializeId);
}