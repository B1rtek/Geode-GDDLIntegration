#include "RatingsManager.h"

#include <Geode/utils/web.hpp>
#include <matjson.hpp>
#include "Utils.h"

using namespace geode::prelude;

std::map<int, GDDLRating> RatingsManager::demonMap;

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
int RatingsManager::getDemonTier(const int id) {
    if (!demonMap.contains(id)) {
        return getCachedTier(id);
    }
    return demonMap[id].roundedRating != -1 ? demonMap[id].roundedRating : demonMap[id].defaultRating;
}

cocos2d::ccColor3B RatingsManager::getTierColor(const int tier) {
    if (tier > Values::tierColors.size() || tier < 0) {
        return ccc3(255, 255, 255);
    }
    const int hexColor = Values::tierColors[tier];
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
    const int correctedRating = rating.roundedRating != -1 ? rating.roundedRating : rating.defaultRating;
    ratingsCache[id] = correctedRating;
    // the requests for ratings are being made inside the rating popups, so the rest of the interface has to "subscribe" to changes
    // this is the place where we can notify them about the update that happened to the ratings list
    for (const auto observer: ratingObservers) {
        observer->updateRating();
    }
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
    cocos2d::CCArray *completedLevels = levelManager->getCompletedLevels(false);
    std::map<int, int> tierStats;
    for(const auto level : CCArrayExt<GJGameLevel *>(completedLevels)) {
        // ReSharper disable once CppTooWideScopeInitStatement
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
 * Checks if cache is empty
 */
bool RatingsManager::cacheEmpty() {
    return ratingsCache.empty();
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

void RatingsManager::subscribeToObservers(IRatingObserver* newSubscriber) {
    ratingObservers.insert(newSubscriber);
}

void RatingsManager::unsubscribeFromObservers(IRatingObserver* unsubscribing) {
    ratingObservers.erase(unsubscribing);
}
