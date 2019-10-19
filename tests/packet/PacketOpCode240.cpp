/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode240
*/

/* Created the 03/10/2019 at 15:26 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(Babel_project, opcode_240,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 240",
        .init=init,
        .fini=fini
)
{
    std::string serializeName = "Charlie";
    std::string deserializeName;
    long serializeDate = 1234;
    long deserializeDate;
    std::string serializeEmail = "charlie.jeanneau@epitech.eu";
    std::string deserializeEmail;
    std::string serializePassword = "lefoot";
    std::string deserializePassword;

    b12software::babel::BabelPacket packet;


    packet.createRegisterNewUserPacket(serializeName, serializeDate, serializeEmail, serializePassword);
    packet.extractRegisterNewUserPacket(deserializeName, deserializeDate, deserializeEmail, deserializePassword);

    cr_assert_eq(packet.getPacketOpCode(), 240, "Expected packet op code to be %u but got %u", 240, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 69, "Expected packet op size to be %lu, but got %lu", 69, packet.getSize());
    cr_assert_eq(deserializeName, serializeName, "Expected deserializedName to be %u, but go %u", deserializeName, serializeName);
    cr_assert_eq(deserializeDate, serializeDate, "Expected deserializedData to be %u, but go %u", deserializeDate, serializeDate);
    cr_assert_eq(deserializeEmail, serializeEmail, "Expected deserializedEmail to be %u, but go %u", deserializeEmail, serializeEmail);
    cr_assert_eq(deserializePassword, serializePassword, "Expected deserializedPassword to be %s, but go %s", deserializePassword.c_str(), serializePassword.c_str());    
}