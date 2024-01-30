#include "RatingsManager.h"

#include <Geode/utils/web.hpp>

using namespace geode::prelude;
using json = nlohmann::json;

std::map<int, GDDLRating> RatingsManager::demonMap;
std::map<int, int> RatingsManager::ratingsCache;

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
    if (response.empty())
        return false;
    GDDLRating rating = parseJson(response);
    demonMap[id] = rating;
    return true;
}
void RatingsManager::cacheRatings(const std::string &response) {
    json ratingsData = json::parse(response);
    for (auto element: ratingsData) {
        const int id = element["ID"];
        const float rating = element["Rating"].is_null() ? -1.0f : static_cast<float>(element["Rating"]);
        const int roundedRating = round(rating);
        ratingsCache[id] = roundedRating;
    }
}

std::map<int, int> RatingsManager::getTierStats() {
    GameLevelManager *levelManager = GameLevelManager::sharedState();
    cocos2d::CCArray *completedLevels = levelManager->getCompletedLevels(false);
    std::map<int, int> tierStats;
    CCObject *obj;
    CCARRAY_FOREACH(completedLevels, obj) {
        auto level = dynamic_cast<GJGameLevel *>(obj);
        if (level->m_normalPercent == 100 && level->m_stars == 10) {
            int cachedTier = ratingsCache[level->m_levelID];
            tierStats[cachedTier]++;
        }
    }
    return tierStats;
}
bool RatingsManager::alreadyCached() {
    return !ratingsCache.empty();
}
