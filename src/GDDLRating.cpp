#include "GDDLRating.h"

GDDLRating::GDDLRating(matjson::Value levelData) {
    this->rating = levelData.contains("Rating") && !levelData["Rating"].is_null() ? levelData["Rating"].as_double() : -1.0f;
    this->enjoyment = levelData.contains("Enjoyment") && !levelData["Enjoyment"].is_null() ? levelData["Enjoyment"].as_double() : -1.0f;
    this->deviation = levelData.contains("Deviation") && !levelData["Deviation"].is_null() ? levelData["Deviation"].as_double() : 0.0;
    this->ratingCount = levelData["RatingCount"].as_int();
    this->enjoymentCount = levelData["EnjoymentCount"].as_int();
    this->submissionCount = levelData["SubmissionCount"].as_int();

    this->roundedRating = static_cast<int>(round(this->rating));
}

GDDLRating GDDLRating::createInvalid() {
    auto rating = GDDLRating();
    rating.ratingCount = -1;
    return rating;
}

bool GDDLRating::isInvalid() const {
    return ratingCount == -1;
}
