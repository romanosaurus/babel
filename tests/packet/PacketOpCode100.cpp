/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** PacketOpCode100.cpp
*/

/* Created the 03/10/2019 at 09:08 by julian.frabel@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_100,
     .timeout=10,
     .description="Test the serialization/deserialization of an opcode 100",
     .init=init,
     .fini=fini
)
{
    bool serializedResponse = true;
    std::string serializedDescription = "this is a test";

    bool deserializedResponse;
    std::string deserializedDescription;

    b12software::babel::BabelPacket packet;

    packet.createSendBinaryResponsePacket(serializedResponse, serializedDescription);
    packet.extractSendBinaryResponsePacket(deserializedResponse, deserializedDescription);

    cr_assert_eq(packet.getPacketOpCode(), 100, "Expected packet op code to be %u but got %u", 100, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 25, "Expected packet op size to be %lu but got %lu", 25, packet.getSize());
    cr_assert_eq(deserializedResponse, serializedResponse, "Expected deserializedResponse to be %u but got %u", deserializedResponse, serializedResponse);
    cr_assert_eq(deserializedDescription, serializedDescription, "Expected deserializedDescription to be '%s' but got '%s'", serializedDescription.c_str(), deserializedDescription.c_str());
}

Test(babel_packet, opcode_100_from_data,
     .timeout=10,
     .description="Test the serialization/deserialization of an opcode 100 from data array",
     .init=init,
     .fini=fini
)
{
    bool serializedResponse = true;
    std::string serializedDescription = "this is a test";

    unsigned char buffer[25] = {
        00, 100, 01, 00, 00, 00, 00, 00, 00, 00, 14, 't', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e', 's', 't'
    };

    bool deserializedResponse;
    std::string deserializedDescription;

    b12software::babel::BabelPacket packet;

    packet.setData(buffer, 25);
    packet.extractSendBinaryResponsePacket(deserializedResponse, deserializedDescription);

    cr_assert_eq(packet.getPacketOpCode(), 100, "Expected packet op code to be %u but got %u", 100, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 25, "Expected packet op size to be %lu but got %lu", 25, packet.getSize());
    cr_assert_eq(deserializedResponse, serializedResponse, "Expected deserializedResponse to be %u but got %u", deserializedResponse, serializedResponse);
    cr_assert_eq(deserializedDescription, serializedDescription, "Expected deserializedDescription to be '%s' but got '%s'", serializedDescription.c_str(), deserializedDescription.c_str());
}