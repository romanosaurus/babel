/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode241
*/

/* Created the 03/10/2019 at 15:51 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_241,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 241",
        .init=init,
        .fini=fini
)
{
    std::string serializeName = "Charlie";
    std::string deserializeName;
    std::string serializePassword = "lefoot";
    std::string deserializePassword;

    b12software::babel::BabelPacket packet;

    packet.createLoginUserPacket(serializeName, serializePassword);
    packet.extractLoginUserPacket(deserializeName, deserializePassword);

    cr_assert_eq(packet.getPacketOpCode(), 241, "Expected packet op code to be %u but got %u", 241, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 26, "Expected packet op size to be %lu, but got %lu", 26, packet.getSize());
    cr_assert_eq(deserializeName, serializeName, "Expected deserializedResponse to be %u, but go %u", deserializeName, serializeName);
    cr_assert_eq(deserializePassword, serializePassword, "Expected deserializedResponse to be %u, but go %u", deserializePassword, serializePassword);    
}