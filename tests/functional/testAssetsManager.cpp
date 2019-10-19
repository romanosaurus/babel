/*
** EPITECH PROJECT, 2019
** babel-v1
** File description:
** testAssetsManager
*/

/* Created the 04/10/2019 at 18:43 by charlie.jeanneau@epitech.eu */

#include <criterion/criterion.h>
#include "../client/sources/gui/utilities/AssetsManager.hpp"
#include <map>

static void init()
{}

static void fini()
{}

Test(test_assets_manager, test_get_asset,
        .timeout=10,
        .description="Test the assetsManager",
        .init=init,
        .fini=fini
)
{
    std::map<std::string, std::string> mapAssets;
    auto &assetsManager = b12software::babel::AssetsManager::getInstance();

    cr_assert_eq(assetsManager.getAsset("avatar"), "./assets/avatar.png");
    cr_assert_eq(assetsManager.getAsset("call_button"), "./assets/call_button.png");
    cr_assert_eq(assetsManager.getAsset("hangup_button"), "./assets/hang_up_button.png");
    cr_assert_eq(assetsManager["loading"], "./assets/spinner.gif");
    cr_assert_eq(assetsManager["delete_button"], "./assets/delete_friend_button.png");
    mapAssets = assetsManager.getAssets();
    cr_assert_eq(mapAssets.size(), assetsManager.getAssets().size());
}