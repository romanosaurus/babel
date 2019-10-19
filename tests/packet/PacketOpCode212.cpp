/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode212
*/

/* Created the 03/10/2019 at 14:52 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_212,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 212",
        .init=init,
        .fini=fini
)
{
    long serializeId = 456;
    long deserialisedId;
    bool serializeAnswer = true;
    bool deserializeAnswer;

    b12software::babel::BabelPacket packet;

    packet.createSendResponseCallPacket(serializeId, serializeAnswer);
    packet.extractSendResponseCallPacket(deserialisedId, deserializeAnswer);

    cr_assert_eq(packet.getPacketOpCode(), 212, "Expected packet op code to be %u but got %u", 212, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 11, "Expected packet op size to be %lu, but got %lu", 11, packet.getSize());
    cr_assert_eq(deserialisedId, serializeId, "Expected deserializedResponse to be %u, but go %u", deserialisedId, serializeId);
    cr_assert_eq(deserializeAnswer, serializeAnswer, "Expected deserializedResponse to be %u, but go %u", deserializeAnswer, serializeAnswer);
}