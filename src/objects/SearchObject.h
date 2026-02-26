#ifndef GDDLINTEGRATION_SEARCHOBJECT_H
#define GDDLINTEGRATION_SEARCHOBJECT_H

#include <memory>
#include <Values.h>

#include "searchsettings/EnumSearchSetting.h"
#include "searchsettings/RangeSearchSetting.h"
#include "searchsettings/TextSearchSetting.h"
#include "searchsettings/BoolSearchSetting.h"

class SearchObject {
    const inline static std::string searchEndpoint = "https://gdladder.com/api/level/search";
    // settings
    std::shared_ptr<EnumSearchSetting> sortSearchSetting = std::make_shared<EnumSearchSetting>(
        "sort", std::vector<std::string>{
            "ID", "Rating", "Name", "Enjoyment", "Enjoyment Count", "Rating Count", "Deviation", "Popularity", "Random"
        }, 0, 0, std::vector<std::string>{
            "ID", "rating", "name", "enjoyment", "enjoymentCount", "ratingCount", "deviation", "popularity", "random"
        });
    std::shared_ptr<TextSearchSetting> levelNameSetting = std::make_shared<TextSearchSetting>("name", "", 32);
    std::shared_ptr<EnumSearchSetting> difficultySetting = std::make_shared<EnumSearchSetting>(
        "difficulty", std::vector<std::string>{
            "Easy Demon", "Medium Demon", "Hard Demon", "Insane Demon", "Extreme Demon", "Any"
        }, 5, 1);
    std::shared_ptr<RangeSearchSetting<int>> tiersSetting = std::make_shared<RangeSearchSetting<int>>(
        "tiers", 0, Values::highestTier, "minRating", "maxRating", std::vector{0, 0});
    std::shared_ptr<RangeSearchSetting<float>> enjoymentsSetting = std::make_shared<RangeSearchSetting<float>>(
        "enjoyments", 0.0f, 10.0f, "minEnjoyment", "maxEnjoyment", std::vector{0.0f, 0.0f});
    std::shared_ptr<BoolSearchSetting> exactNameSetting = std::make_shared<BoolSearchSetting>("exactName", false);
    std::shared_ptr<BoolSearchSetting> removeUnratedSetting = std::make_shared<BoolSearchSetting>(
        "removeUnrated", false);
    std::shared_ptr<BoolSearchSetting> removeRatedSetting = std::make_shared<BoolSearchSetting>("removeRated", false);
    std::vector<std::shared_ptr<ISetting>> settings = {
        sortSearchSetting, levelNameSetting, difficultySetting, tiersSetting, enjoymentsSetting, exactNameSetting, removeUnratedSetting,
        removeRatedSetting
    };
    // current search specific things
    int apiPagesFetched = 0;

public:
    SearchObject() = default;

    void loadSettings();
    void saveSettings();
    void resetToDefaults();
    std::string createSearchQuery();

    std::shared_ptr<EnumSearchSetting> getSortSearchSetting();
    std::shared_ptr<TextSearchSetting> getLevelNameSetting();
    std::shared_ptr<EnumSearchSetting> getDifficultySetting();
    std::shared_ptr<RangeSearchSetting<int>> getTiersSetting();
    std::shared_ptr<RangeSearchSetting<float>> getEnjoymentsSetting();
    std::shared_ptr<BoolSearchSetting> getExactNameSetting();
    std::shared_ptr<BoolSearchSetting> getRemoveUnratedSetting();
    std::shared_ptr<BoolSearchSetting> getRemoveRatedSetting();
};


#endif //GDDLINTEGRATION_SEARCHOBJECT_H
