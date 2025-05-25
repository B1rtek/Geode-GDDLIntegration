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
    if (const auto maybeLevelData = matjson::parse(response); maybeLevelData.isOk()) {
        const matjson::Value& levelData = maybeLevelData.unwrap();
        return GDDLRating(levelData);
    }
    return GDDLRating::createInvalid();
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
    if (const auto maybeData = matjson::parse(content.str()); maybeData.isOk()) {
        matjson::Value data = maybeData.unwrap();
        cacheTimestamp = data["cached"].asInt().unwrapOr(0);
        // ReSharper disable once CppTooWideScopeInitStatement
        const unsigned int currentTimestamp = Utils::getCurrentTimestamp();
        if (currentTimestamp - cacheTimestamp < 86400 * 7) { // list less than 7 days old, load it
            if (data.contains("list") && data["list"].isArray()) {
                for (auto idRatingPair: data["list"].asArray().unwrap()) {
                    const int id = idRatingPair["ID"].asInt().unwrapOr(-1);
                    const int rating = idRatingPair["Rating"].asInt().unwrapOr(-1);
                    ratingsCache[id] = rating;
                }
            }
        }
    }
    // if not - do nothing, the user will be notified that stuff happened
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
    return Utils::hexColorTo3B(hexColor);
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
    // epic csv parser by b1rtek v1.2 (now it doesn't crash the game!!) (i think)
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
        if (values.size() >= 6 && values[0].size() > 2 && values[5].size() > 2) {
            const std::string strID = values[0].substr(1, values[0].size() - 2);
            if (const Result<int> maybeID = numFromString<int>(strID); maybeID.isOk()) {
                const int id = maybeID.unwrap();
                std::string strRating = values[5].substr(1, values[5].size() - 2);
                const float rating = numFromString<float>(strRating).unwrapOr(-1.0f);
                const int roundedRating = static_cast<int>(round(rating));
                ratingsCache[id] = roundedRating;
            }
        }
    }
    if (!ratingsCache.empty()) {
        // don't save the cache if it's empty, that could potentially overwrite an outdated but a potentially full cache
        cacheList(false);
    }
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

/**
 * Populates the in-memory cache from file and then checks if that actually populated it
 */
bool RatingsManager::alreadyCached() {
    populateFromSave();
    return !ratingsCache.empty();
}

/**
 * Checks if cache is empty WIHTOUT trying to populate it
 */
bool RatingsManager::cacheNotEmpty() {
    return !ratingsCache.empty();
}

void RatingsManager::updateCacheFromSearch(const int levelID, const float rating) {
    const int tier = static_cast<int>(std::round(rating));
    ratingsCache[levelID] = tier;
}

int RatingsManager::getCachedTier(const int levelID) {
    return !ratingsCache.contains(levelID) ? -1 : ratingsCache[levelID];
}

void RatingsManager::clearCache() {
    // clears the cache in-memory, leaving the file intact in case the refresh fails for some reason
    ratingsCache.clear();
}

void RatingsManager::cacheSpread(const int levelID, const RatingsSpread& spread) {
    spreadsCache[levelID] = spread;
}

bool RatingsManager::hasSpread(const int levelID) {
    return spreadsCache.contains(levelID);
}

RatingsSpread RatingsManager::getSpread(const int levelID) {
    return spreadsCache[levelID];
}

void RatingsManager::cacheSkillsets(const int levelID, const Skillsets& skillsets) {
    skillsetsCache[levelID] = skillsets;
}

bool RatingsManager::hasSkillsets(const int levelID) {
    return skillsetsCache.contains(levelID);
}

Skillsets RatingsManager::getSkillsets(const int levelID) {
    return skillsetsCache[levelID];
}

void RatingsManager::cacheSubmission(const int levelID, const Submission &submission) {
    submissionsCache[levelID] = submission;
}

bool RatingsManager::hasSubmission(const int levelID) {
    return submissionsCache.contains(levelID);
}

Submission RatingsManager::getSubmission(const int levelID) {
    return submissionsCache[levelID];
}

void RatingsManager::clearSubmissionCache() {
    submissionsCache.clear();
}
