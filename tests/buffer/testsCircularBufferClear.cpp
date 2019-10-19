/*
** EPITECH PROJECT, 2019
** b12software
** File description:
** testsCircularBufferClear.cpp
*/

/* Created the 18/08/2019 at 17:47 by julian.frabel@epitech.eu */

#include <criterion/criterion.h>
#include "b12software/buffer/CircularBuffer.hpp"

static void init()
{}

static void fini()
{}

Test(circularbuffer_clear, clear_buffer,
     .timeout=10,
     .description="Test the clear function of a circular buffer",
     .init=init,
     .fini=fini
)
{
    std::uint8_t to_write[10] = {10};
    b12software::buffer::CircularBuffer buffer(10);

    buffer.write(to_write, 10);
    buffer.clear();
    cr_assert_eq(buffer.getNbBufferedBytes(), 0, "Expected buffered bytes to be 0 after clear but was %lu", buffer.getNbBufferedBytes());
}