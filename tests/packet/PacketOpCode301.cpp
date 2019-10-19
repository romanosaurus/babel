/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode301
*/

/* Created the 03/10/2019 at 16:43 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_301,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 301",
        .init=init,
        .fini=fini
)
{
    long serializeId = 239;
    long deserialiseId;
    bool serializeAnswer = true;
    bool deserializseAnswer;

    b12software::babel::BabelPacket packet;

    packet.createRequestUserContactResponsePacket(serializeId, serializeAnswer);
    packet.extractRequestUserContactResponsePacket(deserialiseId, deserializseAnswer);

    cr_assert_eq(packet.getPacketOpCode(), 301, "Expected packet op code to be %u but got %u", 301, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 11, "Expected packet op size to be %lu, but got %lu", 11, packet.getSize());
    cr_assert_eq(deserialiseId, serializeId, "Expected deserializedId to be %u, but go %u", deserialiseId, serializeId);
    cr_assert_eq(deserializseAnswer, serializeAnswer, "Expected deserializedAnswer to be %u, but go %u", deserializseAnswer, serializeAnswer);
}