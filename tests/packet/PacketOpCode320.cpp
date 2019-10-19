/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode320
*/

/* Created the 03/10/2019 at 18:56 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_320,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 320",
        .init=init,
        .fini=fini
)
{
    long serializeId = 273;
    long deserializeId;
    std::string serializedMessage = "salut ça va ?";
    std::string deserializeMessage;

    b12software::babel::BabelPacket packet;

    packet.createReceivedMessagePacket(serializeId, serializedMessage);
    packet.extractReceivedMessagePacket(deserializeId, deserializeMessage);

    cr_assert_eq(packet.getPacketOpCode(), 320, "Expected packet op code to be %u but got %u", 320, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 32, "Expected packet op size to be %lu, but got %lu", 32, packet.getSize());
    cr_assert_eq(deserializeId, serializeId, "Expected deserializedId to be %u, but go %u", deserializeId, serializeId);
    cr_assert_eq(deserializeMessage, serializedMessage, "Expected deserializedResponse to be %u, but go %u", deserializeMessage, serializedMessage);
}