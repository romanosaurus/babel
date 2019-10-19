/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode200
*/

/* Created the 03/10/2019 at 11:09 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_200,
        .timeout=10,
        .description="Text the serialization/deserialization of an opcode 200",
        .init=init,
        .fini=fini
)
{
    long serializeId = 534;
    long deserializedId;
    b12software::babel::BabelPacket packet;

    packet.createAddUserContactListPacket(serializeId);
    packet.extractAddUserContactListPacket(deserializedId);

    cr_assert_eq(packet.getPacketOpCode(), 200, "Expected packet op code to be %u but got %u", 200, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 10, "Expected packet op size to be %lu, but got %lu", 10, packet.getSize());
    cr_assert_eq(deserializedId, serializeId, "Expected deserializedResponse to be %u but got %u", deserializedId, serializeId);
}