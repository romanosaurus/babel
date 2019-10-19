/*
** EPITECH PROJECT, 2019
** babel-v1
** File description:
** testBabelUser
*/

/* Created the 04/10/2019 at 16:49 by charlie.jeanneau@epitech.eu */

#include <criterion/criterion.h>
#include "BabelUser.hpp"
#include <string>
#include <vector>

static void init()
{}

static void fini()
{}

Test(test_babelUser, get_set,
    .timeout=10,
    .description="Test the BabelUser",
    .init=init,
    .fini=fini
)
{
    b12software::babel::BabelUser userBabel;

    cr_assert_eq(userBabel.isCalling(), false);
    cr_assert_eq(userBabel.isInCall(), false);
    cr_assert_eq(userBabel.isLoggedIn(), false);
}