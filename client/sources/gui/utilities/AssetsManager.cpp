
/* Created by Romain FOUYER on 2019-10-01 at 19:36. */


#include "AssetsManager.hpp"

b12software::babel::AssetsManager::AssetsManager()
{
    _assets["avatar"] = "./assets/avatar.png";
    _assets["call_button"] = "./assets/call_button.png";
    _assets["hangup_button"] = "./assets/hang_up_button.png";
    _assets["loading"] = "./assets/spinner.gif";
    _assets["delete_button"] = "./assets/delete_friend_button.png";
}

b12software::babel::AssetsManager &b12software::babel::AssetsManager::getInstance()
{
    static b12software::babel::AssetsManager instance;

    return instance;
}

std::map<std::string, std::string> b12software::babel::AssetsManager::getAssets() const
{
    return _assets;
}

std::string b12software::babel::AssetsManager::getAsset(const std::string &asset)
{
    return b12software::babel::AssetsManager::getInstance().getAssets().at(asset);
}

const std::string &b12software::babel::AssetsManager::operator[](const std::string &asset)
{
    return _assets[asset];
}