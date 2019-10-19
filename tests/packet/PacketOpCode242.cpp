/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode242
*/

/* Created the 03/10/2019 at 16:01 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_242,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 242",
        .init=init,
        .fini=fini
)
{
    long serializeId = 726;
    long deserializeId;
    std::string serializeName = "Charlie";
    std::string deserializeName;
    long serializeDate = 1234;
    long deserializeDate;
    std::string serializeEmail = "charlie.jeanneau@epitech.eu";
    std::string deserializeEmail;
    std::string serializePassword = "lefoot";
    std::string deserializePassword;

    b12software::babel::BabelPacket packet;

    packet.createUpdateUserPacket(serializeId, serializeName, serializeDate, serializeEmail, serializePassword);
    packet.extractUpdateUserPacket(deserializeId, deserializeName, deserializeDate, deserializeEmail, deserializePassword);

    cr_assert_eq(packet.getPacketOpCode(), 242, "Expected packet op code to be %u but got %u", 242, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 77, "Expected packet op size to be %lu, but got %lu", 77, packet.getSize());
    cr_assert_eq(deserializeId, serializeId, "Expected deserializedResponse got %u, but expected %u", deserializeId, serializeId);
    cr_assert_eq(deserializeName, serializeName, "Expected deserializedResponse got '%s', but expected '%s'", deserializeName.c_str(), serializeName.c_str());
    cr_assert_eq(deserializeDate, serializeDate, "Expected deserializedResponse got %u, but expected %u", deserializeDate, serializeDate);
    cr_assert_eq(deserializeEmail, serializeEmail, "Expected deserializedResponse got '%s', but expected '%s'", deserializeEmail.c_str(), serializeEmail.c_str());
    cr_assert_eq(deserializePassword, serializePassword, "Expected deserializedResponse got '%s', but expected '%s'", deserializePassword.c_str(), serializePassword.c_str());
}