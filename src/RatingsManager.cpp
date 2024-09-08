#include "RatingsManager.h"

#include <Geode/utils/web.hpp>
#include <matjson.hpp>
#include "Utils.h"

using namespace geode::prelude;

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
    try {
        const matjson::Value levelData = matjson::parse(response);
        return GDDLRating(levelData);
    } catch (std::runtime_error &error) {
        return GDDLRating::createInvalid();
    }
}

cocos2d::ccColor3B RatingsManager::convertToColor(const int hexColor) {
    const int r = (hexColor >> (8 * 2)) & 0xff;
    const int g = (hexColor >> (8 * 1)) & 0xff;
    const int b = (hexColor >> (8*0)) & 0xff;
    return ccc3(r, g, b);
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
    if (!Utils::fileExists(cachedListPath)) {
        return;
    }
    std::ifstream f(cachedListPath);
    if (Utils::fileIsEmpty(f)) {
        return;
    }
    std::stringstream content;
    content << f.rdbuf();
    try {
        matjson::Value data = matjson::parse(content.str());
        cacheTimestamp = data["cached"].as_int();
        // ReSharper disable once CppTooWideScopeInitStatement
        const unsigned int currentTimestamp = Utils::getCurrentTimestamp();
        if (currentTimestamp - cacheTimestamp < 86400 * 7) { // list less than 7 days old, load it
            for (auto idRatingPair: data["list"].as_array()) {
                const int id = idRatingPair["ID"].as_int();
                const int rating = idRatingPair["Rating"].as_int();
                ratingsCache[id] = rating;
            }
        }
    } catch (std::runtime_error &error) {
        // just do nothing, the user will be notified that stuff happened
    }
}

void RatingsManager::cacheList(bool onQuit) {
    matjson::Value cachedList;
    if (!onQuit) {
        // update the timestamp because we're saving a new list
        cacheTimestamp = Utils::getCurrentTimestamp(); // NOLINT(*-narrowing-conversions)
        // else: if it was 0 then the request failed and will be tried again on the next game launch
        // otherwise we retain the last value to keep track of the 7 days cache age
    }
    cachedList["cached"] = cacheTimestamp;
    std::vector<matjson::Value> idRatingPairs;
    matjson::Value element;
    for (auto [id, rating]: ratingsCache) {
        element["ID"] = id;
        element["Rating"] = rating;
        idRatingPairs.push_back(element);
    }
    cachedList["list"] = idRatingPairs;
    std::fstream settingsFile;
    settingsFile.open(cachedListPath, std::ios::out);
    settingsFile << cachedList.dump();
    settingsFile.close();
}

// ReSharper disable once CppDFAConstantFunctionResult (it's not true!)
int RatingsManager::getDemonTier(const int id) { return !demonMap.contains(id) ? -1 : demonMap[id].roundedRating; }

cocos2d::ccColor3B RatingsManager::getTierColor(const int tier) {
    if (tier > tierColors.size() || tier < 0) {
        return ccc3(255, 255, 255);
    }
    const int hexColor = tierColors[tier];
    return convertToColor(hexColor);
}

std::optional<GDDLRating> RatingsManager::getRating(const int id) {
    if (!demonMap.contains(id))
        return {};
    return demonMap[id];
}

std::string RatingsManager::getRequestUrl(const int id) {
    std::string requestURL = "https://gdladder.com/api/level/" + std::to_string(id);
    return requestURL;
}

bool RatingsManager::addRatingFromResponse(const int id, const std::string &response) {
    if (response.empty())
        return false;
    const GDDLRating rating = parseJson(response);
    if (rating.isInvalid()) {
        return false;
    }
    demonMap[id] = rating;
    return true;
}

void RatingsManager::cacheRatings(const std::string &response) {
    // ReSharper disable once CppTooWideScopeInitStatement
    // try {
        // now we need to parse csv
        std::stringstream ss;
        std::string value, line;
        ss << response;
        // skip the headers
        std::getline(ss, value);
        // get the data
        while(std::getline(ss, line)) {
            line += ',';
            int linePos = 0, currentQuoteCount = 0;
            std::vector<std::string> values;
            value = "";
            while (linePos < line.size()) {
                if (line[linePos] == '"') {
                    ++currentQuoteCount;
                    value.push_back(line[linePos]);
                } else if (line[linePos] == ',' && currentQuoteCount % 2 == 0) {
                    values.push_back(value);
                    value = "";
                    currentQuoteCount = 0;
                } else {
                    value.push_back(line[linePos]);
                }
                ++linePos;
            }
            // values are in the vector now, we're only interested in the ID and the Rating
            const int id = std::stoi(values[4].substr(1, values[4].size() - 2));
            std::string strRating = values[5].substr(1, values[5].size() - 2);
            if (strRating.empty()) strRating = "-1.0";
            const float rating = std::stof(strRating);
            const int roundedRating = static_cast<int>(round(rating));
            ratingsCache[id] = roundedRating;
        }
        // old code in case /theList comes back
        // matjson::Value ratingsData = matjson::parse(response);
        // for (auto element: ratingsData.as_array()) {
        //     const int id = element["ID"].as_int();
        //     const float rating = element["Rating"].is_null() ? -1.0f : element["Rating"].as_double();
        //     const int roundedRating = static_cast<int>(round(rating));
        //     ratingsCache[id] = roundedRating;
        // }
        cacheList(false);
    // } catch (std::runtime_error &error) {
    //     // just do nothing, the user will be notified that stuff happened
    // }
}

std::map<int, int> RatingsManager::getTierStats() {
    GameLevelManager *levelManager = GameLevelManager::sharedState();
    const cocos2d::CCArray *completedLevels = levelManager->getCompletedLevels(false);
    std::map<int, int> tierStats;
    CCObject *obj;
    CCARRAY_FOREACH(completedLevels, obj) {
        // ReSharper disable once CppTooWideScopeInitStatement
        const auto level = dynamic_cast<GJGameLevel *>(obj);
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

void RatingsManager::updateCacheFromSearch(const int levelID, const float rating) {
    const int tier = static_cast<int>(std::round(rating));
    ratingsCache[levelID] = tier;
}

int RatingsManager::getCachedTier(const int levelID) {
    return !ratingsCache.contains(levelID) ? -1 : ratingsCache[levelID];
}
