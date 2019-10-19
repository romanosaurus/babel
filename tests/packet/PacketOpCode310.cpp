/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode310
*/

/* Created the 03/10/2019 at 18:22 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_310,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 310",
        .init=init,
        .fini=fini
)
{
    long serializeId = 928;
    long deserialisedId;
    int serializeIpv4 = 2;
    int deserializeIpv4;
    int serializePort = 4444;
    int deserializePort;

    b12software::babel::BabelPacket packet;

    packet.createReceivedCallPacket(serializeId, serializeIpv4, serializePort);
    packet.extractReceivedCallPacket(deserialisedId, deserializeIpv4, deserializePort);

    cr_assert_eq(packet.getPacketOpCode(), 310, "Expected packet op code to be %u but got %u", 310, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 18, "Expected packet op size to be %lu, but got %lu", 18, packet.getSize());
    cr_assert_eq(deserialisedId, serializeId, "Expected deserializedId to be %u, but go %u", deserialisedId, serializeId);
    cr_assert_eq(deserializeIpv4, serializeIpv4, "Expected deserializedIpv4 to be %u, but go %u", deserializeIpv4, serializeIpv4);
    cr_assert_eq(deserializePort, serializePort, "Expected deserializedPort to be %u, but go %u", deserializePort, serializePort);
}