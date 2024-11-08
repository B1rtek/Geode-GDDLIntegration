#include "GDDLRating.h"

GDDLRating::GDDLRating(matjson::Value levelData) {
    this->rating = levelData.contains("Rating") && !levelData["Rating"].is_null() ? levelData["Rating"].as_double() : -1.0f;
    this->enjoyment = levelData.contains("Enjoyment") && !levelData["Enjoyment"].is_null() ? levelData["Enjoyment"].as_double() : -1.0f;
    this->deviation = levelData.contains("Deviation") && !levelData["Deviation"].is_null() ? levelData["Deviation"].as_double() : 0.0;
    this->ratingCount = levelData.contains("RatingCount") && !levelData["RatingCount"].is_null() ? levelData["RatingCount"].as_int() : 0;
    this->enjoymentCount = levelData.contains("EnjoymentCount") && !levelData["EnjoymentCount"].is_null() ? levelData["EnjoymentCount"].as_int() : 0;
    this->submissionCount = levelData.contains("SubmissionCount") && !levelData["SubmissionCount"].is_null() ? levelData["SubmissionCount"].as_int() : 0;
    this->showcaseVideoID = levelData.contains("Showcase") && !levelData["Showcase"].is_null() ? levelData["Showcase"].as_string() : "";

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
