/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode203
*/

/* Created the 03/10/2019 at 14:07 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_203,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 203",
        .init=init,
        .fini=fini
)
{
    long serializeId = 345;
    long deserializedId;
    bool serializeAnswer = true;
    bool deserializeAnswer;

    b12software::babel::BabelPacket packet;

    packet.createSendResponseContactRequestPacket(serializeId, serializeAnswer);
    packet.extractSendResponseContactRequestPacket(deserializedId, deserializeAnswer);

    cr_assert_eq(packet.getPacketOpCode(), 203, "Expected packet op code to be %u but got %u", 203, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 11, "Expected packet op size to be %lu, but got %lu", 11, packet.getSize());
    cr_assert_eq(deserializedId, serializeId, "Expected deserializedResponse to be %u, but go %u", deserializedId, serializeId);
    cr_assert_eq(deserializeAnswer, serializeAnswer, "Expected deserializedResponse to be %u, but go %u", deserializeAnswer, serializeAnswer);
}