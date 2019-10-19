/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode300
*/

/* Created the 03/10/2019 at 16:17 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_300,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 300",
        .init=init,
        .fini=fini
)
{
    long serializeId = 738;
    long deserialiseId;

    b12software::babel::BabelPacket packet;

    packet.createRequestContactRequestPacket(serializeId);
    packet.extractRequestContactRequestPacket(deserialiseId);

    cr_assert_eq(packet.getPacketOpCode(), 300, "Expected packet op code to be %u but got %u", 300, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 10, "Expected packet op size to be %lu, but got %lu", 10, packet.getSize());
    cr_assert_eq(deserialiseId, serializeId, "Expected deserializedResponse to be %u, but go %u", deserialiseId, serializeId);
}