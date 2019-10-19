/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode221
*/

/* Created the 03/10/2019 at 15:15 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_projet, opcode_221,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 221",
        .init=init,
        .fini=fini
)
{
    long serializeId = 900;
    long deserialisedId;
    long serializeStart = 10;
    long deserialisedStart;
    long serializeEnd = 20;
    long deserializeEnd;

    b12software::babel::BabelPacket packet;

    packet.createRetrieveMessageListPacket(serializeId, serializeStart, serializeEnd);
    packet.extractRetrieveMessageListPacket(deserialisedId, deserialisedStart, deserializeEnd);

    cr_assert_eq(packet.getPacketOpCode(), 221, "Expected packet op code to be %u but got %u", 221, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 26, "Expected packet op size to be %lu, but got %lu", 26, packet.getSize());
    cr_assert_eq(deserialisedId, serializeId, "Expected deserializedId got %u, but expected %u", deserialisedId, serializeId);
    cr_assert_eq(deserialisedStart, serializeStart, "Expected deserializedStart got %u, but expected %u", deserialisedStart, serializeStart);
    cr_assert_eq(deserializeEnd, serializeEnd, "Expected deserializedEnd got %u, but expected %u", deserializeEnd, serializeEnd);
}
