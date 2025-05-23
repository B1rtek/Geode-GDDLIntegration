#include "RatingsSpread.h"
#include "RatingsManager.h"
#include "Utils.h"

RatingsSpread::RatingsSpread(const matjson::Value &spreadJson) {
    if (const int currentTime = time(nullptr); currentTime < Utils::API_SWITCH_TIME) {
        if (!spreadJson.contains("rating") || !spreadJson.contains("enjoyment") || !spreadJson["rating"].isArray() || !spreadJson["enjoyment"].isArray()) {
            return;
        }
        const auto ratings = spreadJson["rating"].asArray().unwrap();
        const auto enjoyments = spreadJson["enjoyment"].asArray().unwrap();
        for (const auto &diffRating: ratings) {
            if (!diffRating.contains("Rating") || !diffRating.contains("Count") || !diffRating["Rating"].isNumber() ||
                !diffRating["Count"].isNumber()) {
                continue;
                }
            const int rating = diffRating["Rating"].asInt().unwrap();
            const int count = diffRating["Count"].asInt().unwrap();
            minRating = std::min(minRating, rating);
            maxRating = std::max(maxRating, rating);
            diffRatings[rating] = count;
        }
        for (const auto &enjRating: enjoyments) {
            if (!enjRating.contains("Enjoyment") || !enjRating.contains("Count") || !enjRating["Enjoyment"].isNumber() ||
                !enjRating["Count"].isNumber()) {
                continue;
                }
            const int enjoyment = enjRating["Enjoyment"].asInt().unwrap();
            const int count = enjRating["Count"].asInt().unwrap();
            minEnj = std::min(minEnj, enjoyment);
            maxEnj = std::max(maxEnj, enjoyment);
            enjRatings[enjoyment] = count;
        }
    } else {
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
