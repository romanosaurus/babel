/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode340
*/

/* Created the 04/10/2019 at 11:25 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_340,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 340",
        .init=init,
        .fini=fini
)
{
    long serializeId = 364;
    long deserializeId;

    b12software::babel::BabelPacket packet;

    packet.createUserAuthResponsePacket(serializeId);
    packet.extractUserAuthResponsePacket(deserializeId);

    cr_assert_eq(packet.getPacketOpCode(), 340, "Expected packet op code to be %u but got %u", 340, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 10, "Expected packet op size to be %lu, but got %lu", 10, packet.getSize());
    cr_assert_eq(deserializeId, serializeId, "Expected deserializedNbContact to be %u, but go %u", deserializeId, serializeId);
}