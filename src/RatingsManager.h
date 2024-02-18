#ifndef RATINGSMANAGER_H
#define RATINGSMANAGER_H

#include <map>
#include <Geode/Geode.hpp>
#include <json.hpp>

#include "GDDLRating.h"

using namespace geode::prelude;

enum TierSearchType {
    ANY,
    UNCOMPLETED,
    COMPLETED
};

class RatingsManager {
    static std::map<int, GDDLRating> demonMap;
    static std::vector<int> tierColors;
    static std::map<int, int> ratingsCache;
    inline static std::string cachedListPath = Mod::get()->getSaveDir().string() + "/gddlcache.json";
    inline static int searchedTier = -1;
    inline static TierSearchType tierSearchType{};
    inline static bool searchingForTier = false;
    inline static std::vector<int> searchResults{};

    static GDDLRating parseJson(const std::string& response);

    static cocos2d::ccColor3B convertToColor(int hexColor);

    static void populateFromSave();

    static void cacheList();

    static void prepareSearchResults(int tier, TierSearchType searchType);

public:
    static int getDemonTier(int id);

    static cocos2d::ccColor3B getTierColor(int tier);

    static std::optional<GDDLRating> getRating(int id);

    static std::string getRequestUrl(int id);

    static bool addRatingFromResponse(int id, const std::string &response);

    static void cacheRatings(const std::string &response);

    static std::map<int, int> getTierStats();

    static bool alreadyCached();

    static void setupSearch(int tier, TierSearchType searchType);

    static bool isSearchingForTier();

    static GJSearchObject *getSearchPage(int page);

    static int getSearchResultsPageCount();

    static int getSearchResultsCount();

    static void stopSearch();

    static void updateCacheFromSearch(int levelID, int tier);

    static int getCachedTier(int levelID);
};


#endif // RATINGSMANAGER_H
