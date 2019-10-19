/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode201
*/

/* Created the 03/10/2019 at 11:44 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_201,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 201",
        .init=init,
        .fini=fini
)
{
    long serializeId = 789;
    long deserialized;
    b12software::babel::BabelPacket packet;

    packet.createDeleteUserContactListPacket(serializeId);
    packet.extractDeleteUserContactListPacket(deserialized);

    cr_assert_eq(packet.getPacketOpCode(), 201, "Expected packet op code to be %u but got %u", 201, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 10, "Expected packet op size to be %lu, but got %lu", 10, packet.getSize());
    cr_assert_eq(deserialized, serializeId, "Expected deserializedResponse to be %u, but go %u", deserialized, serializeId);
}