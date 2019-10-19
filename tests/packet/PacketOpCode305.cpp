/*
** EPITECH PROJECT, 2019
** test
** File description:
** PacketOpCode303
*/

/* Created the 03/10/2019 at 17:34 by charlie.jeanneau@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "protocol/BabelPacket.hpp"

static void init()
{}

static void fini()
{}

Test(babel_packet, opcode_305,
        .timeout=10,
        .description="Test the serialization/deserialization of an opcode 305",
        .init=init,
        .fini=fini
)
{
    std::vector<long> serializeNbContact { 1,2 };
    std::vector<long> deserialiseNbContact;

    b12software::babel::BabelPacket packet;

    packet.createListContactRequestListResponsePacket(serializeNbContact);
    packet.extractListContactRequestListResponsePacket(deserialiseNbContact);

    cr_assert_eq(packet.getPacketOpCode(), 305, "Expected packet op code to be %u but got %u", 305, packet.getPacketOpCode());
    cr_assert_eq(packet.getSize(), 26, "Expected packet op size to be %lu, but got %lu", 26, packet.getSize());
    cr_assert_eq(deserialiseNbContact.size(), serializeNbContact.size(), "Expected deserializedNbContact to be %u, but go %u", deserialiseNbContact.size(), serializeNbContact.size());
    for (size_t i = 0; i < serializeNbContact.size(); i++) {
        cr_assert_eq(serializeNbContact[i], deserialiseNbContact[i]);
    }
}