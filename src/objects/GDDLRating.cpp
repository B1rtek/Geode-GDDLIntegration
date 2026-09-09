#include "GDDLRating.h"

#include <cmath>

GDDLRating::GDDLRating(matjson::Value levelData) {
    this->rating = levelData.contains("Rating") && !levelData["Rating"].isNull() && levelData["Rating"].isNumber() ? levelData["Rating"].asDouble().unwrap() : -1.0f;
    this->defaultRating = levelData.contains("DefaultRating") && !levelData["DefaultRating"].isNull() && levelData["DefaultRating"].isNumber() ? levelData["DefaultRating"].asInt().unwrap() : -1;
    this->enjoyment = levelData.contains("Enjoyment") && !levelData["Enjoyment"].isNull() && levelData["Enjoyment"].isNumber() ? levelData["Enjoyment"].asDouble().unwrap() : -1.0f;
    this->deviation = levelData.contains("Deviation") && !levelData["Deviation"].isNull() && levelData["Deviation"].isNumber() ? levelData["Deviation"].asDouble().unwrap() : 0.0;
    this->ratingCount = levelData.contains("RatingCount") && !levelData["RatingCount"].isNull() && levelData["RatingCount"].isNumber() ? levelData["RatingCount"].asInt().unwrap() : 0;
    this->enjoymentCount = levelData.contains("EnjoymentCount") && !levelData["EnjoymentCount"].isNull() && levelData["EnjoymentCount"].isNumber() ? levelData["EnjoymentCount"].asInt().unwrap() : 0;
    this->submissionCount = levelData.contains("SubmissionCount") && !levelData["SubmissionCount"].isNull() && levelData["SubmissionCount"].isNumber() ? levelData["SubmissionCount"].asInt().unwrap() : 0;
    this->showcaseVideoID = levelData.contains("Showcase") && !levelData["Showcase"].isNull() && levelData["Showcase"].isString() ? levelData["Showcase"].asString().unwrap() : "";

    this->roundedRating = static_cast<int>(std::round(this->rating));

}

GDDLRating GDDLRating::createInvalid() {
    auto rating = GDDLRating();
    rating.ratingCount = -1;
    return rating;
}

bool GDDLRating::isInvalid() const {
    return ratingCount == -1;
}
