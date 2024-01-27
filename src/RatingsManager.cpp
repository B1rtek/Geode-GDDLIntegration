#include "RatingsManager.h"

#include <Geode/utils/web.hpp>

using namespace geode::prelude;
using json = nlohmann::json;

std::map<int, GDDLRating> RatingsManager::demonMap;

GDDLRating RatingsManager::parseJson(std::string response) {
    json levelData = json::parse(response);
    return GDDLRating(levelData);
}

std::string RatingsManager::requestRating(int id) {
    // it's a simple request so I don't think that async is needed here
    std::string requestURL = "https://gdladder.com/api/level?levelID=" + std::to_string(id);
    auto res = web::fetch(requestURL);
    if (!res) {
        return "";
    }
    return res.value();
}

int RatingsManager::getDemonTier(int id) {
    if (!demonMap.contains(id)) {
        std::string response = requestRating(id);
        if (response.empty()) return -1;
        GDDLRating rating = parseJson(response);
        demonMap[id] = rating;
    }
    return demonMap[id].roundedRating;
}
