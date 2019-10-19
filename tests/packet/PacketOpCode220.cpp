/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode220
*/

/* Created the 03/10/2019 at 15:01 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_220,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 220",
        .init=init,
        .fini=fini
)
{
    long serializeId = 888;
    long deserialisedId;
    std::string serializeMessage = "salut";
    std::string deserializeMessage;

    b12software::babel::BabelPacket packet;
    
    packet.createSendMessagePacket(serializeId, serializeMessage);
    packet.extractSendMessagePacket(deserialisedId, deserializeMessage);

    cr_assert_eq(packet.getPacketOpCode(), 220, "Expected packet op code to be %u but got %u", 220, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 23, "Expected packet op size to be %lu, but got %lu", 23, packet.getSize());
    cr_assert_eq(deserialisedId, serializeId, "Expected deserializedResponse to be %u, but go %u", deserialisedId, serializeId);
    cr_assert_eq(deserializeMessage, serializeMessage, "Expected deserializedResponse to be %u, but go %u", deserializeMessage, serializeMessage);
}