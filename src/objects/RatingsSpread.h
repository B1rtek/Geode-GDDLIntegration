#ifndef GDDLINTEGRATION_RATINGSSPREAD_H
#define GDDLINTEGRATION_RATINGSSPREAD_H

#include <map>
#include <vector>
#include "nodes/BarChartNode.h"

class RatingsSpread {
    std::map<int, int> diffRatings{};
    std::map<int, int> enjRatings{};
    int minRating = 36, maxRating = -1, minEnj = 11, maxEnj = -1;
    const inline static int enjColors[] = {
            0xe67c73,
            0xea8b71,
            0xee9a6d,
            0xf2a96d,
            0xf6b86b,
            0xfac769,
            0xffd666,
            0xd5d06f,
            0xabc978,
            0x81c281,
            0x57bb8a
    };

public:
    RatingsSpread() = default;

    explicit RatingsSpread(const matjson::Value& spreadJson);

    std::vector<BarChartData> getDiffSpreadData();
    std::vector<BarChartData> getEnjSpreadData();
};


#endif //GDDLINTEGRATION_RATINGSSPREAD_H
