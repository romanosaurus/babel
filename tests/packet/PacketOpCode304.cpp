/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode304
*/

/* Created the 03/10/2019 at 17:47 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_304,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 304",
        .init=init,
        .fini=fini
)
{
    long serializedUserId = 38;
    long deserializedUserId;
    std::string serializeName = "Charlie";
    std::string deserialisedName;
    long serializeBirthData = 234;
    long deserialisedBirthData;
    std::string serializeMail = "charlie.jeanneau@epitech.eu";
    std::string deserialisedMail;

    b12software::babel::BabelPacket packet;
    
    packet.createListContactInfoResponsePacket(serializedUserId, serializeName, serializeBirthData, serializeMail);
    packet.extractListContactInfoResponsePacket(deserializedUserId, deserialisedName, deserialisedBirthData, deserialisedMail);

    cr_assert_eq(packet.getPacketOpCode(), 304, "Expected packet op code to be %u but got %u", 304, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 63, "Expected packet op size to be %lu, but got %lu", 63, packet.getSize());
    cr_assert_eq(deserializedUserId, serializedUserId, "Expected deserializedUserid got %u, but expected %u", deserializedUserId, serializedUserId);
    cr_assert_eq(deserialisedName, serializeName, "Expected deserializedName got %s, but expected %s", deserialisedName.c_str(), serializeName.c_str());
    cr_assert_eq(deserialisedBirthData, serializeBirthData, "Expected deserializedBirthData got %u, but expected %u", deserialisedBirthData, serializeBirthData);
    cr_assert_eq(deserialisedMail, serializeMail, "Expected deserializedMail got %s, but expected %s", deserialisedMail.c_str(), serializeMail.c_str());
}