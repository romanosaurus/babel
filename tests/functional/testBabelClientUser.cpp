/*
** EPITECH PROJECT, 2019
** babel-v1
** File description:
** testBabelClientUser
*/

/* Created the 04/10/2019 at 18:28 by charlie.jeanneau@epitech.eu */

#include <criterion/criterion.h>
#include "../client/sources/gui/BabelClientUser.hpp"

static void init()
{}

static void fini()
{}

Test(test_babelClientUser, get_set,
        .timeout=10,
        .description="Test the BabelClientUser",
        .init=init,
        .fini=fini
)
{
    int userId = 1;
    b12software::babel::BabelClientUser client("bozeti", "lePathDeMonImage", "monEmail", userId);

    cr_assert_eq(client.getId(), 1);
    cr_assert_eq(client.getAvatarPath(), "lePathDeMonImage");
    cr_assert_eq(client.getEmailAddress(), "monEmail");
    cr_assert_eq(client.getUsername(), "bozeti");
}