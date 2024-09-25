#ifndef RATINGSMANAGER_H
#define RATINGSMANAGER_H

#include <map>
#include <Geode/Geode.hpp>

#include "GDDLRating.h"

using namespace geode::prelude;

class RatingsManager {
    static std::map<int, GDDLRating> demonMap;
    static std::vector<int> tierColors;
    static std::map<int, int> ratingsCache;
    inline static int cacheTimestamp = 0;
    inline static std::string cachedListPath = Mod::get()->getSaveDir().string() + "/gddlcache.json";

    static GDDLRating parseJson(const std::string& response);

    static cocos2d::ccColor3B convertToColor(int hexColor);

public:
    inline static std::string gddlSheetUrl = "https://docs.google.com/spreadsheets/d/1qKlWKpDkOpU1ZF6V6xGfutDY2NvcA8MNPnsv6GBkKPQ/gviz/tq?tqx=out:csv&sheet=GDDL";

    inline static bool triedToCache = false;

    static void populateFromSave();

    static int getDemonTier(int id);

    static cocos2d::ccColor3B getTierColor(int tier);

    static std::optional<GDDLRating> getRating(int id);

    static std::string getRequestUrl(int id);

    static bool addRatingFromResponse(int id, const std::string &response);

    static void cacheRatings(const std::string &response);

    static std::map<int, int> getTierStats();

    static bool alreadyCached();

    static bool cacheNotEmpty();

    static void updateCacheFromSearch(int levelID, const float rating);

    static int getCachedTier(int levelID);

    static void cacheList(bool onQuit);

    static void clearCache();
};


#endif // RATINGSMANAGER_H
