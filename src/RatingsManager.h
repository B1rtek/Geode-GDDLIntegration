#ifndef RATINGSMANAGER_H
#define RATINGSMANAGER_H

#include <map>

#include <json.hpp>

#include "GDDLRating.h"

using namespace geode::prelude;

class RatingsManager {
    static std::map<int, GDDLRating> demonMap;
    static std::map<int, int> ratingsCache;
    inline static std::string cachedListPath = Mod::get()->getSaveDir().string() + "/gddlcache.json";

    static GDDLRating parseJson(std::string response);

    static void populateFromSave();

    static void cacheList();

public:
    static int getDemonTier(int id);
    static std::optional<GDDLRating> getRating(int id);

    static std::string getRequestUrl(int id);

    static bool addRatingFromResponse(int id, std::string response);

    static void cacheRatings(const std::string &response);

    static std::map<int, int> getTierStats();

    static bool alreadyCached();
};


#endif // RATINGSMANAGER_H
