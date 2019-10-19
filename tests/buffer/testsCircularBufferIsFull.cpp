/*
** EPITECH PROJECT, 2019
** b12software
** File description:
** testsCircularBufferIsFull.cpp
*/

/* Created the 18/08/2019 at 17:49 by julian.frabel@epitech.eu */

#include <criterion/criterion.h>
#include "b12software/buffer/CircularBuffer.hpp"

static void init()
{}

static void fini()
{}

Test(circularbuffer_is_full, is_full_false,
     .timeout=10,
     .description="Test the is full function of a non full circular buffer",
     .init=init,
     .fini=fini
)
{
    std::uint8_t to_write[6] = {6};
    b12software::buffer::CircularBuffer buffer(10);

    buffer.write(to_write, 6);
    cr_assert_eq(buffer.isFull(), false, "Expected isFull to return false but returned true");
}

Test(circularbuffer_is_full, is_full_true,
     .timeout=10,
     .description="Test the is full function of a full circular buffer",
     .init=init,
     .fini=fini
)
{
    std::uint8_t to_write[10] = {6};
    b12software::buffer::CircularBuffer buffer(10);

    buffer.write(to_write, 10);
    cr_assert_eq(buffer.isFull(), true, "Expected isFull to return true but returned false");
}

Test(circularbuffer_is_full, is_full_after_overflow,
     .timeout=10,
     .description="Test the is full function of a full overflowed circular buffer",
     .init=init,
     .fini=fini
)
{
    std::uint8_t to_write[11] = {6};
    b12software::buffer::CircularBuffer buffer(10);

    buffer.write(to_write, 11);
    cr_assert_eq(buffer.isFull(), true, "Expected isFull to return true but returned false");
}

Test(circularbuffer_is_full, is_full_after_reading,
     .timeout=10,
     .description="Test the is full function of a readen circular buffer",
     .init=init,
     .fini=fini
)
{
    std::uint8_t to_write[10] = {6};
    std::uint8_t to_read[10];
    b12software::buffer::CircularBuffer buffer(10);

    buffer.write(to_write, 10);
    cr_assert_eq(buffer.isFull(), true, "Expected isFull to return true but returned false");
    buffer.read(to_read, 1);
    cr_assert_eq(buffer.isFull(), false, "Expected isFull to return false but returned true");
}

Test(circularbuffer_is_full, is_full_after_reading_overflow,
     .timeout=10,
     .description="Test the is full function of a readed overflowed circular buffer",
     .init=init,
     .fini=fini
)
{
    std::uint8_t to_write[11] = {6};
    std::uint8_t to_read[10];
    b12software::buffer::CircularBuffer buffer(10);

    buffer.write(to_write, 11);
    cr_assert_eq(buffer.isFull(), true, "Expected isFull to return true but returned false");
    buffer.read(to_read, 1);
    cr_assert_eq(buffer.isFull(), false, "Expected isFull to return false but returned true");
}