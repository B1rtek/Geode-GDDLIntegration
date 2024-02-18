#include "RatingsManager.h"

#include <Geode/utils/web.hpp>
#include <chrono>
#include "Utils.h"

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

std::map<int, int> RatingsManager::ratingsCache;

GDDLRating RatingsManager::parseJson(const std::string& response) {
    json levelData = json::parse(response);
    return GDDLRating(levelData);
}

cocos2d::ccColor3B RatingsManager::convertToColor(int hexColor) {
    int r = (hexColor >> (8*2)) & 0xff;
    int g = (hexColor >> (8*1)) & 0xff;
    int b = (hexColor >> (8*0)) & 0xff;
    return cocos2d::ccColor3B(r, g, b);
}

/**
 * Cached json format:
 * {
 *  cached: unixTimestamp;
 *  list: [{ID: int, Rating: int}] (list returned from the GDDL API)
 * }
 *
 * If the list is over a week old, don't load data from it
 */
void RatingsManager::populateFromSave() {
    if (!Utils::fileExists(cachedListPath))
        return;
    std::ifstream f(cachedListPath);
    json data = json::parse(f);
    const unsigned int cachedTimestamp = data["cached"];
    const unsigned int currentTimestamp = Utils::getCurrentTimestamp();
    if (currentTimestamp - cachedTimestamp < 86400 * 7) { // list less than 7 days old, load it
        for (auto idRatingPair: data["list"]) {
            const int id = idRatingPair["ID"];
            const int rating = idRatingPair["Rating"];
            ratingsCache[id] = rating;
        }
    }
}

void RatingsManager::cacheList() {
    json cachedList;
    cachedList["cached"] = Utils::getCurrentTimestamp();
    std::vector<json> idRatingPairs;
    json element;
    for (auto [id, rating]: ratingsCache) {
        element["ID"] = id;
        element["Rating"] = rating;
        idRatingPairs.push_back(element);
    }
    cachedList["list"] = idRatingPairs;
    std::fstream settingsFile;
    settingsFile.open(cachedListPath, std::ios::out);
    settingsFile << cachedList;
    settingsFile.close();
}

void RatingsManager::prepareSearchResults(const int tier, TierSearchType searchType) {
    std::set<int> allLevelsFromTier;
    for (auto [id, cachedTier]: ratingsCache) {
        if (cachedTier == tier) {
            allLevelsFromTier.insert(id);
        }
    }
    if (searchType != ANY) {
        std::set<int> allCompletedTierLevels;
        GameLevelManager *levelManager = GameLevelManager::sharedState();
        const cocos2d::CCArray *completedLevels = levelManager->getCompletedLevels(false);
        CCObject *obj;
        CCARRAY_FOREACH(completedLevels, obj) {
            const auto level = dynamic_cast<GJGameLevel *>(obj);
            // ReSharper disable once CppTooWideScopeInitStatement
            const bool levelCompleted = level->m_normalPercent == 100;
            if (allLevelsFromTier.contains(level->m_levelID) && levelCompleted) {
                allCompletedTierLevels.insert(level->m_levelID);
            }
        }
        if (searchType == COMPLETED) {
            searchResults = Utils::copySetToVector(allCompletedTierLevels);
        } else { // searchType == UNCOMPLETED
            for (auto level: allCompletedTierLevels) {
                allLevelsFromTier.erase(level);
            }
            searchResults = Utils::copySetToVector(allLevelsFromTier);
        }
    } else {
        searchResults = Utils::copySetToVector(allLevelsFromTier);
    }
}

// ReSharper disable once CppDFAConstantFunctionResult (it's not true!)
int RatingsManager::getDemonTier(int id) { return !demonMap.contains(id) ? -1 : demonMap[id].roundedRating; }

cocos2d::ccColor3B RatingsManager::getTierColor(int tier) {
    if(tier > tierColors.size() || tier < 0) {
        return cocos2d::ccColor3B(255, 255, 255);
    }
    int hexColor = tierColors[tier];
    return convertToColor(hexColor);
}

std::optional<GDDLRating> RatingsManager::getRating(int id) {
    if (!demonMap.contains(id))
        return {};
    return demonMap[id];
}

std::string RatingsManager::getRequestUrl(int id) {
    std::string requestURL = "https://gdladder.com/api/level?levelID=" + std::to_string(id);
    return requestURL;
}

bool RatingsManager::addRatingFromResponse(int id, const std::string &response) {
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
        const int roundedRating = static_cast<int>(round(rating));
        ratingsCache[id] = roundedRating;
    }
    cacheList();
}

std::map<int, int> RatingsManager::getTierStats() {
    GameLevelManager *levelManager = GameLevelManager::sharedState();
    cocos2d::CCArray *completedLevels = levelManager->getCompletedLevels(false);
    std::map<int, int> tierStats;
    CCObject *obj;
    CCARRAY_FOREACH(completedLevels, obj) {
        auto level = dynamic_cast<GJGameLevel *>(obj);
        if (level->m_normalPercent == 100 && level->m_stars == 10) {
            int cachedTier = -1;
            if (ratingsCache.contains(level->m_levelID)) {
                cachedTier = ratingsCache[level->m_levelID];
            }
            tierStats[cachedTier]++;
        }
    }
    return tierStats;
}
bool RatingsManager::alreadyCached() {
    populateFromSave();
    return !ratingsCache.empty();
}

void RatingsManager::setupSearch(const int tier, const TierSearchType searchType) {
    prepareSearchResults(tier, searchType);
    searchingForTier = true;
}

bool RatingsManager::isSearchingForTier() {
    return searchingForTier;
}

GJSearchObject *RatingsManager::getSearchPage(int page) {
    if (page < 1) {
        page = 1;
    } else if (page > getSearchResultsPageCount()) {
        page = getSearchResultsPageCount();
    }
    const int firstIndex = std::max(0, (page - 1) * 10);
    const int lastIndex = std::min(firstIndex+10, static_cast<int>(searchResults.size()));
    std::string requestString;
    for (int i = firstIndex; i < lastIndex; i++) {
        requestString += std::to_string(searchResults[i]) + ',';
    }
    if (!requestString.empty()) {
        requestString.pop_back();
    }
    requestString += "&gameVersion=22";
    return GJSearchObject::create(SearchType::Type19, requestString);
}

int RatingsManager::getSearchResultsPageCount() {
    const unsigned int searchResultsCount = searchResults.size();
    const int correction = searchResultsCount % 10 == 0 ? 0 : 1;
    return searchResultsCount / 10 + correction; // NOLINT(*-narrowing-conversions)
}

int RatingsManager::getSearchResultsCount() {
    return searchResults.size();
}

void RatingsManager::stopSearch() { searchingForTier = false; }

void RatingsManager::updateCacheFromSearch(const int levelID, const int tier) { ratingsCache[levelID] = tier; }

int RatingsManager::getCachedTier(const int levelID) {
    return !ratingsCache.contains(levelID) ? -1 : ratingsCache[levelID];
}
