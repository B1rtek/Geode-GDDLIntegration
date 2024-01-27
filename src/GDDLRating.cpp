#include "GDDLRating.h"

GDDLRating::GDDLRating(json levelData) {
    this->rating = levelData["Rating"];
    this->enjoyment = levelData["Enjoyment"];
    this->deviation = levelData["Deviation"];
    this->ratingCount = levelData["RatingCount"];
    this->enjoymentCount = levelData["EnjoymentCount"];
    this->submissionCount = levelData["SubmissionCount"];

    this->roundedRating = round(this->rating);
}
