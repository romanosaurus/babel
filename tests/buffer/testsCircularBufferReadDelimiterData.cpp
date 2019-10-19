/*
** EPITECH PROJECT, 2019
** b12software
** File description:
** testsCircularBufferReadDelimiterData.cpp
*/

/* Created the 05/09/2019 at 18:58 by julian.frabel@epitech.eu */

#include <criterion/criterion.h>
#include "b12software/buffer/CircularBuffer.hpp"

static void init()
{}

static void fini()
{}

Test(circularbuffer_read, read_delimiter_data,
     .timeout=10,
     .description="Test the reading with a delimiter from the buffer",
     .init=init,
     .fini=fini
)
{
    const char *expect = "blihblih\n\r";
    const char *write  = "blihblih\n\rbloh\n";
    int size = 15;
    b12software::buffer::CircularBuffer buffer(25);
    std::string str;

    buffer.write(write, size);
    size_t readed = buffer.read(str, "\n\r", 2);
    cr_assert_eq(readed, 10, "Expected to read %lu bytes but got %lu.", 10, readed);
    cr_assert_str_eq(str.c_str(), expect, "Expected [%s] but got [%s]", expect, str.c_str());
}