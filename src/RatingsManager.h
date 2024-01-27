#ifndef RATINGSMANAGER_H
#define RATINGSMANAGER_H

#include <map>

#include <json.hpp>

#include "GDDLRating.h"

class RatingsManager {
    static std::map<int, GDDLRating> demonMap;

    static GDDLRating parseJson(std::string response);

    static std::string requestRating(int id);

public:
    static int getDemonTier(int id);
};



#endif //RATINGSMANAGER_H
