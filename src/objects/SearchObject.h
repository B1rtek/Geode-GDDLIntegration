#ifndef GDDLINTEGRATION_SEARCHOBJECT_H
#define GDDLINTEGRATION_SEARCHOBJECT_H

#include <memory>
#include <Values.h>

#include "Skillsets.h"
#include "searchsettings/EnumSearchSetting.h"
#include "searchsettings/RangeSearchSetting.h"
#include "searchsettings/TextSearchSetting.h"
#include "searchsettings/BoolSearchSetting.h"

class SearchObject {
    const inline static std::string searchEndpoint = "https://gdladder.com/api/level/search";
    // settings
    std::shared_ptr<EnumSearchSetting> sortSetting = std::make_shared<EnumSearchSetting>(
        "sort", std::vector<std::string>{
            "ID", "Rating", "Name", "Enjoyment", "Enjoyment Count", "Rating Count", "Deviation", "Popularity", "Random"
        }, 0, 0, std::vector<std::string>{
            "ID", "rating", "name", "enjoyment", "enjoymentCount", "ratingCount", "deviation", "popularity", "random"
        });
    std::shared_ptr<EnumSearchSetting> sortDirectionSetting = std::make_shared<EnumSearchSetting>(
        "sortDirection", std::vector<std::string>{
            "Ascending", "Descending"
        }, 0, 0, std::vector<std::string>{
            "asc", "desc"
        });
    std::shared_ptr<TextSearchSetting> levelNameSetting = std::make_shared<TextSearchSetting>("name", "", 32);
    std::shared_ptr<TextSearchSetting> creatorNameSetting = std::make_shared<TextSearchSetting>("creator", "", 32);
    std::shared_ptr<TextSearchSetting> songNameSetting = std::make_shared<TextSearchSetting>("song", "", 64);
    std::shared_ptr<RangeSearchSetting<int>> ratingsSetting = std::make_shared<RangeSearchSetting<int>>(
        "ratings", 0, Values::highestTier, "minRating", "maxRating", std::vector{0, 0});
    std::shared_ptr<RangeSearchSetting<float>> enjoymentsSetting = std::make_shared<RangeSearchSetting<float>>(
        "enjoyments", 0.0f, 10.0f, "minEnjoyment", "maxEnjoyment", std::vector{0.0f, 0.0f});
    std::shared_ptr<RangeSearchSetting<int>> enjoymentsCountSetting = std::make_shared<RangeSearchSetting<int>>(
        "enjoymentsCount", 0, 9999, "minEnjoymentCount", "maxEnjoymentCount", std::vector{0, 0});
    std::shared_ptr<RangeSearchSetting<float>> deviationsSetting = std::make_shared<RangeSearchSetting<float>>(
        "deviations", 0.0f, 100.0f, "minDeviation", "maxDeviation", std::vector{0.0f, 0.0f});
    std::shared_ptr<RangeSearchSetting<int>> ratingsCountSetting = std::make_shared<RangeSearchSetting<int>>(
        "ratingsCount", 0, 9999, "minSubmissionCount", "maxSubmissionCount", std::vector{0, 0});
    std::shared_ptr<EnumSearchSetting> difficultySetting = std::make_shared<EnumSearchSetting>(
        "difficulty", std::vector<std::string>{
            "Easy Demon", "Medium Demon", "Hard Demon", "Insane Demon", "Extreme Demon", "Any"
        }, 5, 1);
    std::shared_ptr<RangeSearchSetting<int>> idsRangeSetting = std::make_shared<RangeSearchSetting<int>>(
        "idsRange", 0, 2000000000, "minID", "maxID", std::vector{0, 0});
    std::shared_ptr<EnumSearchSetting> lengthSetting = std::make_shared<EnumSearchSetting>(
        "length", std::vector<std::string>{
            "Any", "Tiny", "Short", "Medium", "Long", "XL", "Platformer"
        });
    std::shared_ptr<EnumSearchSetting> twoPlayerSetting = std::make_shared<EnumSearchSetting>(
        "twoPlayer", std::vector<std::string>{
            "Any", "Not 2P mode", "Only 2P mode"
        }, 0, 0, std::vector<std::string>{
            "any", "no", "only"
        });
    std::shared_ptr<BoolSearchSetting> isInPackSetting = std::make_shared<BoolSearchSetting>("isInPack", false);
    std::shared_ptr<EnumSearchSetting> topSkillsetSetting = std::make_shared<EnumSearchSetting>("topSkillset", Skillsets::skillsetNames);
    std::shared_ptr<EnumSearchSetting> hasSkillsetSetting = std::make_shared<EnumSearchSetting>("hasSkillset", Skillsets::skillsetNames);
    // skipping notRatedBy, I don't really think anybody would use this apart from maybe checking theirs? and it would be trickier to implement so nope not now
    std::shared_ptr<BoolSearchSetting> completedSetting = std::make_shared<BoolSearchSetting>("excludeCompleted", false); // special, not included in searchableSettings vector
    std::shared_ptr<BoolSearchSetting> uncompletedSetting = std::make_shared<BoolSearchSetting>("excludeUncompleted", false); // special, not included in searchableSettings vector
    std::shared_ptr<BoolSearchSetting> excludeRatedSetting = std::make_shared<BoolSearchSetting>("excludeRated", false);
    std::shared_ptr<BoolSearchSetting> excludeRatedEnjoymentSetting = std::make_shared<BoolSearchSetting>("excludeRatedEnjoyment", false);
    std::shared_ptr<BoolSearchSetting> excludeUnratedSetting = std::make_shared<BoolSearchSetting>("excludeUnrated", false);
    std::shared_ptr<BoolSearchSetting> excludeUnratedEnjoymentSetting = std::make_shared<BoolSearchSetting>("excludeUnratedEnjoyment", false);
    std::vector<std::shared_ptr<ISetting>> settings = {
        sortSetting, sortDirectionSetting, levelNameSetting, creatorNameSetting, songNameSetting,
        ratingsSetting, enjoymentsSetting, enjoymentsCountSetting, deviationsSetting, ratingsCountSetting,
        difficultySetting, idsRangeSetting, lengthSetting, twoPlayerSetting, isInPackSetting, topSkillsetSetting,
        hasSkillsetSetting, completedSetting, uncompletedSetting, excludeRatedSetting,
        excludeRatedEnjoymentSetting, excludeUnratedSetting, excludeUnratedEnjoymentSetting
    };
    std::vector<std::shared_ptr<ISetting>> searchableSettings = {
        sortSetting, sortDirectionSetting, levelNameSetting, creatorNameSetting, songNameSetting,
        ratingsSetting, enjoymentsSetting, enjoymentsCountSetting, deviationsSetting, ratingsCountSetting,
        difficultySetting, idsRangeSetting, lengthSetting, twoPlayerSetting, isInPackSetting, topSkillsetSetting,
        hasSkillsetSetting, excludeRatedSetting, excludeRatedEnjoymentSetting, excludeUnratedSetting,
        excludeUnratedEnjoymentSetting
    };
    // current search specific things
    int apiPagesFetched = 0;

public:
    SearchObject() = default;

    void loadSettings();
    void saveSettings();
    void resetToDefaults();
    std::string createSearchQuery();

    std::shared_ptr<EnumSearchSetting> getSortSetting();
    std::shared_ptr<EnumSearchSetting> getSortDirectionSetting();
    std::shared_ptr<TextSearchSetting> getLevelNameSetting();
    std::shared_ptr<TextSearchSetting> getCreatorNameSetting();
    std::shared_ptr<TextSearchSetting> getSongNameSetting();
    std::shared_ptr<RangeSearchSetting<int>> getRatingsSetting();
    std::shared_ptr<RangeSearchSetting<float>> getEnjoymentsSetting();
    std::shared_ptr<RangeSearchSetting<int>> getEnjoymentsCountSetting();
    std::shared_ptr<RangeSearchSetting<float>> getDeviationsSetting();
    std::shared_ptr<RangeSearchSetting<int>> getRatingsCountSetting();
    std::shared_ptr<EnumSearchSetting> getDifficultySetting();
    std::shared_ptr<RangeSearchSetting<int>> getIdsRangeSetting();
    std::shared_ptr<EnumSearchSetting> getLengthSetting();
    std::shared_ptr<EnumSearchSetting> getTwoPlayerSetting();
    std::shared_ptr<BoolSearchSetting> getIsInPackSetting();
    std::shared_ptr<EnumSearchSetting> getTopSkillsetSetting();
    std::shared_ptr<EnumSearchSetting> getHasSkillsetSetting();
    std::shared_ptr<BoolSearchSetting> getCompletedSetting();
    std::shared_ptr<BoolSearchSetting> getUncompletedSetting();
    std::shared_ptr<BoolSearchSetting> getExcludeRatedSetting();
    std::shared_ptr<BoolSearchSetting> getExcludeRatedEnjoymentSetting();
    std::shared_ptr<BoolSearchSetting> getExcludeUnratedSetting();
    std::shared_ptr<BoolSearchSetting> getExcludeUnratedEnjoymentSetting();
};


#endif //GDDLINTEGRATION_SEARCHOBJECT_H
