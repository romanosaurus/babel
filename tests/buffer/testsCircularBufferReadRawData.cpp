/*
** EPITECH PROJECT, 2019
** b12software
** File description:
** testsCircularBufferReadRawData.cpp
*/

/* Created the 18/08/2019 at 16:30 by julian.frabel@epitech.eu */

#include <iostream>
#include <criterion/criterion.h>
#include "b12software/buffer/CircularBuffer.hpp"
#include "b12software/buffer/NotEnoughDataInCircularBufferException.hpp"
#include "b12software/exception/NullPointerException.hpp"

static void init()
{}

static void fini()
{}

Test(circularbuffer_read, read_raw_data,
     .timeout=10,
     .description="Test the reading to raw data from the buffer",
     .init=init,
     .fini=fini
)
{
    constexpr size_t BUFF_SIZE = 6;
    std::uint8_t write_from[BUFF_SIZE] = {0, 1, 2, 3, 4, 5};
    std::uint8_t write_to[BUFF_SIZE] = {0};
    b12software::buffer::CircularBuffer buffer(10);

    buffer.write(write_from, BUFF_SIZE);
    size_t readed = buffer.read(write_to, BUFF_SIZE);
    cr_assert_eq(readed, BUFF_SIZE, "Expected to read %lu bytes but got %lu.", BUFF_SIZE, readed);
    cr_assert_eq(std::memcmp(write_from, write_to, BUFF_SIZE), 0, "Expected readed data to be the same as writen data (can be an error in write or read function)");
}

Test(circularbuffer_read, read_raw_data_after_overflow,
     .timeout=10,
     .description="Test the reading to raw data from the buffer after overflow",
     .init=init,
     .fini=fini
)
{
    std::uint8_t write_from[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    std::uint8_t write_to[4] = {0};
    std::uint8_t expected[4] = {2, 3, 4, 5};
    b12software::buffer::CircularBuffer buffer(10);

    buffer.write(write_from, 12);
    size_t readed = buffer.read(write_to, 4);
    cr_assert_eq(readed, 4, "Expected to read %lu bytes but got %lu.", 4, readed);
    cr_assert_eq(std::memcmp(expected, write_to, 4), 0, "Expected readed data to be the same as writen data (can be an error in write or read function)");
}

Test(circularbuffer_read, read_more_data_that_hold_data,
     .timeout=10,
     .description="Test the reading of more data than what's hold",
     .init=init,
     .fini=fini
)
{
    constexpr size_t BUFF_SIZE = 6;
    size_t readed;
    std::uint8_t write_from[BUFF_SIZE] = {0, 1, 2, 3, 4, 5};
    std::uint8_t write_to[BUFF_SIZE] = {0};
    b12software::buffer::CircularBuffer buffer(10);

    buffer.write(write_from, BUFF_SIZE);
    try {
        readed = buffer.read(write_to, 10);
    } catch (b12software::buffer::NotEnoughDataInCircularBufferException &e) {
        return;
    }
    cr_assert_fail("Was expecting a NotEnoughDataInCircularBufferException but got %lu bytes readed instead.", readed);
}

Test(circularbuffer_read, read_data_to_null_pointer,
     .timeout=10,
     .description="Test the reading of data to a null buffer",
     .init=init,
     .fini=fini
)
{
    constexpr size_t BUFF_SIZE = 6;
    size_t readed;
    std::uint8_t write_from[BUFF_SIZE] = {0, 1, 2, 3, 4, 5};
    std::uint8_t *write_to = nullptr;
    b12software::buffer::CircularBuffer buffer(10);

    buffer.write(write_from, BUFF_SIZE);
    try {
        readed = buffer.read(write_to, BUFF_SIZE);
    } catch (b12software::exception::NullPointerException &e) {
        return;
    }
    cr_assert_fail("Was expecting a NullPointerException but got %lu bytes readed instead.", readed);
}