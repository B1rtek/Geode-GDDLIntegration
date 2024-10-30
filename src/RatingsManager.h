#ifndef RATINGSMANAGER_H
#define RATINGSMANAGER_H

#include <map>
#include <Geode/Geode.hpp>
#include <objects/RatingsSpread.h>
#include <objects/Skillsets.h>

#include "objects/GDDLRating.h"

using namespace geode::prelude;

class RatingsManager {
    static std::map<int, GDDLRating> demonMap;
    static std::map<int, int> ratingsCache;
    inline static std::map<int, RatingsSpread> spreadsCache{};
    inline static std::map<int, Skillsets> skillsetsCache{};
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

    static void cacheSpread(const int levelID, const RatingsSpread& spread);

    static bool hasSpread(const int levelID);

    static RatingsSpread getSpread(const int levelID);

    static void cacheSkillsets(const int levelID, const Skillsets& skillsets);

    static bool hasSkillsets(const int levelID);

    static Skillsets getSkillsets(const int levelID);

    static std::vector<int> tierColors;
};


#endif // RATINGSMANAGER_H
