#ifndef RATINGSMANAGER_H
#define RATINGSMANAGER_H

#include <map>

#include <json.hpp>

#include "GDDLRating.h"

class RatingsManager {
    static std::map<int, GDDLRating> demonMap;
    static std::vector<int> tierColors;

    static GDDLRating parseJson(std::string response);

    static std::string requestRating(int id);

    static cocos2d::ccColor3B convertToColor(int hexColor);

public:
    static int getDemonTier(int id);

    static cocos2d::ccColor3B getTierColor(int tier);
};



#endif //RATINGSMANAGER_H
