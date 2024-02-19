#include "GDDLRating.h"

GDDLRating::GDDLRating(json levelData) {
    this->rating = !levelData["Rating"].is_null() ? static_cast<float>(levelData["Rating"]) : -1.0f;
    this->enjoyment = !levelData["Enjoyment"].is_null() ? static_cast<float>(levelData["Enjoyment"]) : -1.0f;
    this->deviation = !levelData["Deviation"].is_null() ? static_cast<double>(levelData["Deviation"]) : 0.0;
    this->ratingCount = levelData["RatingCount"];
    this->enjoymentCount = levelData["EnjoymentCount"];
    this->submissionCount = levelData["SubmissionCount"];

    this->roundedRating = static_cast<int>(round(this->rating));
}
