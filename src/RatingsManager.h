#ifndef RATINGSMANAGER_H
#define RATINGSMANAGER_H

#include <map>
#include <Geode/Geode.hpp>
#include <json.hpp>

#include "GDDLRating.h"

using namespace geode::prelude;

class RatingsManager {
    static std::map<int, GDDLRating> demonMap;
    static std::vector<int> tierColors;
    static std::map<int, int> ratingsCache;
    inline static std::string cachedListPath = Mod::get()->getSaveDir().string() + "/gddlcache.json";

    static GDDLRating parseJson(const std::string& response);

    static cocos2d::ccColor3B convertToColor(int hexColor);

    static void populateFromSave();

public:
    static int getDemonTier(int id);

    static cocos2d::ccColor3B getTierColor(int tier);

    static std::optional<GDDLRating> getRating(int id);

    static std::string getRequestUrl(int id);

    static bool addRatingFromResponse(int id, const std::string &response);

    static void cacheRatings(const std::string &response);

    static std::map<int, int> getTierStats();

    static bool alreadyCached();

    static void updateCacheFromSearch(int levelID, const float rating);

    static int getCachedTier(int levelID);

    static void cacheList();
};


#endif // RATINGSMANAGER_H
