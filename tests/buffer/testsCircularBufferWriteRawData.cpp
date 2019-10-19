/*
** EPITECH PROJECT, 2019
** b12software
** File description:
** testsCircularBufferWriteRawData.cpp
*/

/* Created the 08/08/2019 at 19:56 by julian.frabel@epitech.eu */

#include <criterion/criterion.h>
#include "b12software/buffer/CircularBuffer.hpp"
#include "b12software/exception/NullPointerException.hpp"

static void init()
{}

static void fini()
{}

Test(circularbuffer_write, raw_data,
     .timeout=10,
     .description="Test the write of raw data to the buffer",
     .init=init,
     .fini=fini
)
{
    std::uint8_t data[6] = {0, 1, 2, 3, 4, 5};
    b12software::buffer::CircularBuffer buffer(10);

    cr_assert_eq(buffer.write(data, sizeof(data)), false, "Buffer returned an overflow but wasn't expecting overflow");
}

Test(circularbuffer_write, raw_data_fill_buffer,
     .timeout=10,
     .description="Test the write of raw data to the buffer and fill it without overflow",
     .init=init,
     .fini=fini
)
{
    std::uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    b12software::buffer::CircularBuffer buffer(10);

    cr_assert_eq(buffer.write(data, sizeof(data)), false, "Buffer returned an overflow when full but wasn't expecting overflow");
}

Test(circularbuffer_write, raw_data_overflow_buffer,
     .timeout=10,
     .description="Test the write of raw data to the buffer and overflow it",
     .init=init,
     .fini=fini
)
{
    std::uint8_t data[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    b12software::buffer::CircularBuffer buffer(10);

    cr_assert_eq(buffer.write(data, sizeof(data)), true, "Buffer did not return an overflow but was expecting one");
}

Test(circularbuffer_write, null_data_pointer,
     .timeout=10,
     .description="Test the write of raw data to the buffer with a null pointer",
     .init=init,
     .fini=fini
)
{
    void *data = nullptr;
    b12software::buffer::CircularBuffer buffer(10);

    try {
        buffer.write(data, 1);
    } catch (b12software::exception::NullPointerException &e) {
        return;
    }
    cr_assert_fail("Was expecting a NullPointerException but got none");
}