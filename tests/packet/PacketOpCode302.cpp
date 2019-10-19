/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode302
*/

/* Created the 03/10/2019 at 17:27 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_302,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 302",
        .init=init,
        .fini=fini
)
{
    long serializeId = 293;
    long deserialiseId;

    b12software::babel::BabelPacket packet;

    packet.createDeletedUserContactListResponsePacket(serializeId);
    packet.extractDeletedUserContactListResponsePacket(deserialiseId);

    cr_assert_eq(packet.getPacketOpCode(), 302, "Expected packet op code to be %u but got %u", 302, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 10, "Expected packet op size to be %lu, but got %lu", 10, packet.getSize());
    cr_assert_eq(deserialiseId, serializeId, "Expected deserializedId to be %u, but go %u", deserialiseId, serializeId);
}