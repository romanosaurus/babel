/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode321
*/

/* Created the 04/10/2019 at 11:13 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode321,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 321",
        .init=init,
        .fini=fini
)
{
    long serializeId = 293;
    long deserializedId;
    std::vector<std::pair<uint64_t, std::string>> serializeAllName {{1, "charlie"}, {2, "szlt"}};
    std::vector<std::pair<uint64_t, std::string>> deserializedAllName;

    b12software::babel::BabelPacket packet;

    packet.createRetrieveMessageListResponsePacket(serializeId, serializeAllName);
    packet.extractRetrieveMessageListResponsePacket(deserializedId, deserializedAllName);

    cr_assert_eq(packet.getPacketOpCode(), 321, "Expected packet op code to be %u but got %u", 321, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 61, "Expected packet op size to be %lu, but got %lu", 61, packet.getSize());
    cr_assert_eq(deserializedId, serializeId, "Expected deserializedId to be %u, but go %u", deserializedId, serializeId);
    cr_assert_eq(deserializedAllName.size(), serializeAllName.size(), "Expected vector size to be %lu, but got %lu", deserializedAllName.size(), serializeAllName.size());
    for (size_t i = 0; i < serializeAllName.size(); i++) {
        cr_assert_eq(serializeAllName[i].first, deserializedAllName[i].first);
        cr_assert_eq(serializeAllName[i].second, deserializedAllName[i].second);
    }
}