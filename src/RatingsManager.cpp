#include "RatingsManager.h"

#include <Geode/utils/web.hpp>

using namespace geode::prelude;
using json = nlohmann::json;

std::map<int, GDDLRating> RatingsManager::demonMap;
std::vector<int> RatingsManager::tierColors = {
    0x360432,
    0xdddfee,
    0xd5d3e9,
    0xd3cbe7,
    0xd3c3e4,
    0xd4bbe2,
    0xd5b0de,
    0xdba7dc,
    0xdb9fd1,
    0xd991c1,
    0xda86b0,
    0xdb7698,
    0xdc6a7d,
    0xdd5a5a,
    0xdc514c,
    0xda493e,
    0xda4534,
    0xd93e26,
    0xdc3f23,
    0xc03a1b,
    0xb23415,
    0xa23010,
    0x9b2b0c,
    0x932b0b,
    0x892608,
    0x832607,
    0x752105,
    0x6c1e04,
    0x601a02,
    0x5a1802,
    0x511700,
    0x491900,
    0x3f1a00,
    0x331700,
    0x231300,
    0x110a00,
    0x000000
};

GDDLRating RatingsManager::parseJson(std::string response) {
    json levelData = json::parse(response);
    return GDDLRating(levelData);
}

cocos2d::ccColor3B RatingsManager::convertToColor(int hexColor) {
    int r = (hexColor >> (8*2)) & 0xff;
    int g = (hexColor >> (8*1)) & 0xff;
    int b = (hexColor >> (8*0)) & 0xff;
    return cocos2d::ccColor3B(r, g, b);
}

int RatingsManager::getDemonTier(int id) {
    return !demonMap.contains(id) ? -1 : demonMap[id].roundedRating;
}

std::optional<GDDLRating> RatingsManager::getRating(int id) {
    if (!demonMap.contains(id)) return {};
    return demonMap[id];
}

std::string RatingsManager::getRequestUrl(int id) {
    std::string requestURL = "https://gdladder.com/api/level?levelID=" + std::to_string(id);
    return requestURL;
}

cocos2d::ccColor3B RatingsManager::getTierColor(int tier) {
    if(tier > tierColors.size() || tier < 0) {
        return cocos2d::ccColor3B(255, 255, 255);
    }
    int hexColor = tierColors[tier];
    return convertToColor(hexColor);
}

bool RatingsManager::addRatingFromResponse(int id, std::string response) {
    if (response.empty()) return false;
    GDDLRating rating = parseJson(response);
    demonMap[id] = rating;
    return true;
}
