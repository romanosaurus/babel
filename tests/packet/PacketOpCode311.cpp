/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode311
*/

/* Created the 03/10/2019 at 18:37 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_311,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 311",
        .init=init,
        .fini=fini
)
{
    long serializeId = 928;
    long deserialisedId;
    bool serializeAnswer = true;
    bool deserializeAnswer;
    int serializeIpv4 = 2;
    int deserializeIpv4;
    int serializePort = 4444;
    int deserializePort;

    b12software::babel::BabelPacket packet;

    packet.createCallUserResponsePacket(serializeId, serializeAnswer, serializeIpv4, serializePort);
    packet.extractCallUserResponsePacket(deserialisedId, deserializeAnswer, deserializeIpv4, deserializePort);

    cr_assert_eq(packet.getPacketOpCode(), 311, "Expected packet op code to be %u but got %u", 311, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 19, "Expected packet op size to be %lu, but got %lu", 19, packet.getSize());
    cr_assert_eq(deserialisedId, serializeId, "Expected deserializedId to be %u, but go %u", deserialisedId, serializeId);
    cr_assert_eq(deserializeAnswer, serializeAnswer, "Expected deserializedAnswer to be %u, but go %u", deserializeAnswer, serializeAnswer);
    cr_assert_eq(deserializeIpv4, serializeIpv4, "Expected deserializedIpv4 to be %u, but go %u", deserializeIpv4, serializeIpv4);
    cr_assert_eq(deserializePort, serializePort, "Expected deserializedPort to be %u, but go %u", deserializePort, serializePort);
}