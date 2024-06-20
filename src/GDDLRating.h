#ifndef GDDLRATING_H
#define GDDLRATING_H

#include <matjson.hpp>

class GDDLRating {
public:
    explicit GDDLRating(matjson::Value levelData);

    GDDLRating() = default;

    float rating;
    int roundedRating;
    float enjoyment;
    double deviation;
    int ratingCount;
    int enjoymentCount;
    int submissionCount;

    static GDDLRating createInvalid();

    bool isInvalid() const;
};


#endif //GDDLRATING_H
