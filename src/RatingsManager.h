#ifndef RATINGSMANAGER_H
#define RATINGSMANAGER_H

#include <map>

#include <json.hpp>

#include "GDDLRating.h"

class RatingsManager {
    static std::map<int, GDDLRating> demonMap;

    static GDDLRating parseJson(std::string response);

public:
    static int getDemonTier(int id);
    static std::optional<GDDLRating> getRating(int id);

    static std::string getRequestUrl(int id);

    static bool addRatingFromResponse(int id, std::string response);
};



#endif //RATINGSMANAGER_H
