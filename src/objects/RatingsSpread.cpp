#include "RatingsSpread.h"
#include "RatingsManager.h"
#include "Utils.h"

RatingsSpread::RatingsSpread(const matjson::Value &spreadJson) {
    if (!spreadJson.contains("rating") || !spreadJson.contains("enjoyment") || !spreadJson["rating"].isObject() || !spreadJson["enjoyment"].isObject()) {
        return;
    }
    const auto ratings = spreadJson["rating"];
    const auto enjoyments = spreadJson["enjoyment"];
    for (const auto &[key, value]: ratings) {
        const Result<int> maybeRating = numFromString<int>(key);
        if (maybeRating.isOk()) {
            const int rating = maybeRating.unwrap();
            const int count = value.asInt().unwrapOr(0);
            minRating = std::min(minRating, rating);
            maxRating = std::max(maxRating, rating);
            diffRatings[rating] = count;
        }
    }
    for (const auto &[key, value]: enjoyments) {
        const Result<int> maybeEnj = numFromString<int>(key);
        if (maybeEnj.isOk()) {
            const int enjoyment = maybeEnj.unwrap();
            const int count = value.asInt().unwrapOr(0);
            minEnj = std::min(minEnj, enjoyment);
            maxEnj = std::max(maxEnj, enjoyment);
            enjRatings[enjoyment] = count;
        }
    }
}

std::vector<BarChartData> RatingsSpread::getDiffSpreadData() {
    std::vector<BarChartData> chartData{};
    for (int i = minRating; i <= maxRating; i++) {
        const int count = diffRatings.contains(i) ? diffRatings[i] : 0;
        chartData.push_back(BarChartData(Utils::hexColorTo4F(RatingsManager::tierColors[i]), "Tier " + std::to_string(i), count));
    }
    return chartData;
}

std::vector<BarChartData> RatingsSpread::getEnjSpreadData() {
    std::vector<BarChartData> chartData{};
    for (int i = minEnj; i <= maxEnj; i++) {
        const int count = enjRatings.contains(i) ? enjRatings[i] : 0;
        chartData.push_back(BarChartData(Utils::hexColorTo4F(enjColors[i]), std::to_string(i) + "/10", count));
    }
    return chartData;
}
