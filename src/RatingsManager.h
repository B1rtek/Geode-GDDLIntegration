#ifndef RATINGSMANAGER_H
#define RATINGSMANAGER_H

#include <map>

#include <json.hpp>

#include "GDDLRating.h"

class RatingsManager {
    static std::map<int, GDDLRating> demonMap;
    static std::map<int, int> ratingsCache;

    static GDDLRating parseJson(std::string response);

public:
    static int getDemonTier(int id);
    static std::optional<GDDLRating> getRating(int id);

    static std::string getRequestUrl(int id);

    static bool addRatingFromResponse(int id, std::string response);

    static void cacheRatings(const std::string &response);

    static std::map<int, int> getTierStats();
};


#endif // RATINGSMANAGER_H
