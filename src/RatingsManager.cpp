#include "RatingsManager.h"

#include <Geode/utils/web.hpp>

using namespace geode::prelude;
using json = nlohmann::json;

std::map<int, GDDLRating> RatingsManager::demonMap;

GDDLRating RatingsManager::parseJson(std::string response) {
    json levelData = json::parse(response);
    return GDDLRating(levelData);
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

bool RatingsManager::addRatingFromResponse(int id, std::string response) {
    if (response.empty()) return false;
    GDDLRating rating = parseJson(response);
    demonMap[id] = rating;
    return true;
}
