/*
** EPITECH PROJECT, 2019
** babel-v1
** File description:
** testUserUtilities
*/

/* Created the 04/10/2019 at 18:53 by charlie.jeanneau@epitech.eu */

#include <criterion/criterion.h>
#include "../client/sources/gui/utilities/UserUtilities.hpp"

static void init()
{}

static void fini()
{}

Test(test_userUtilities, set_get,
        .timeout=10,
        .description="Test the UserUtilities",
        .init=init,
        .fini=fini
)
{
    int newId = 83;
    auto &userUtilities = b12software::babel::UserUtilities::getInstance();

    cr_assert_eq(userUtilities.getCalledId(), -1);

    userUtilities.setCalledId(newId);

    cr_assert_eq(userUtilities.getCalledId(), newId);
}