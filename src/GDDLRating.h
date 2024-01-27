#ifndef GDDLRATING_H
#define GDDLRATING_H

#include <json.hpp>

using json = nlohmann::json;

class GDDLRating {
public:
    explicit GDDLRating(json levelData);

    float rating;
    int roundedRating;
    float enjoyment;
    double deviation;
    int ratingCount;
    int enjoymentCount;
    int submissionCount;
};


#endif //GDDLRATING_H
