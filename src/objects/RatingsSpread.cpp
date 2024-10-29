#include "RatingsSpread.h"
#include "RatingsManager.h"
#include "Utils.h"

RatingsSpread::RatingsSpread(const matjson::Value &spreadJson) {
    if (!spreadJson.contains("rating") || !spreadJson.contains("enjoyment") || !spreadJson["rating"].is_array() ||
        !spreadJson["enjoyment"].is_array()) {
        return;
    }

    const auto ratings = spreadJson["rating"].as_array();
    const auto enjoyments = spreadJson["enjoyment"].as_array();
    for (const auto &diffRating: ratings) {
        if (!diffRating.contains("Rating") || !diffRating.contains("Count") || !diffRating["Rating"].is_number() ||
            !diffRating["Count"].is_number()) {
            continue;
        }
        const int rating = diffRating["Rating"].as_int();
        const int count = diffRating["Count"].as_int();
        diffRatings[rating] = count;
    }
    for (const auto &enjRating: enjoyments) {
        if (!enjRating.contains("Enjoyment") || !enjRating.contains("Count") || !enjRating["Enjoyment"].is_number() ||
            !enjRating["Count"].is_number()) {
            continue;
        }
        const int enjoyment = enjRating["Enjoyment"].as_int();
        const int count = enjRating["Count"].as_int();
        enjRatings[enjoyment] = count;
    }
}

std::vector<BarChartData> RatingsSpread::getDiffSpreadData() {
    const int minRating = diffRatings.begin()->first;
    const int maxRating = diffRatings.end()->first;
    std::vector<BarChartData> chartData;
    for (int i = minRating; i <= maxRating; i++) {
        const int count = diffRatings.contains(i) ? diffRatings[i] : 0;
        chartData.push_back(BarChartData(Utils::hexColorTo4F(RatingsManager::tierColors[i]), "Tier " + std::to_string(i), count));
    }
    return chartData;
}

std::vector<BarChartData> RatingsSpread::getEnjSpreadData() {
    const int minEnj = enjRatings.begin()->first;
    const int maxEnj = enjRatings.end()->first;
    std::vector<BarChartData> chartData;
    for (int i = minEnj; i <= maxEnj; i++) {
        const int count = enjRatings.contains(i) ? enjRatings[i] : 0;
        chartData.push_back(BarChartData(Utils::hexColorTo4F(enjColors[i]), std::to_string(i) + "/10", count));
    }
    return chartData;
}
