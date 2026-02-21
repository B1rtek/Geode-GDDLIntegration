#ifndef GDDLINTEGRATION_SEARCHOBJECT_H
#define GDDLINTEGRATION_SEARCHOBJECT_H

#include <memory>
#include <Values.h>

#include "searchsettings/EnumSearchSetting.h"
#include "searchsettings/RangeSearchSetting.h"
#include "searchsettings/TextSearchSetting.h"

class SearchObject {
    std::shared_ptr<TextSearchSetting> levelNameSetting = std::make_shared<TextSearchSetting>("search-name", "", 32);
    std::shared_ptr<EnumSearchSetting> difficultySetting = std::make_shared<EnumSearchSetting>("search-difficulty", std::vector<std::string>{"Easy Demon", "Medium Demon", "Hard Demon", "Insane Demon", "Extreme Demon", "Any"}, 5);
    std::shared_ptr<RangeSearchSetting<int>> tiersSetting = std::make_shared<RangeSearchSetting<int>>("search-tiers", 0, Values::highestTier);
    std::shared_ptr<RangeSearchSetting<float>> enjoymentsSetting = std::make_shared<RangeSearchSetting<float>>("search-enjoyments", 0.0f, 10.0f);

public:
    SearchObject() = default;

    void loadSettings();
    void saveSettings();

    std::shared_ptr<TextSearchSetting> getLevelNameSetting();
    std::shared_ptr<EnumSearchSetting> getDifficultySetting();
    std::shared_ptr<RangeSearchSetting<int>> getTiersSetting();
    std::shared_ptr<RangeSearchSetting<float>> getEnjoymentsSetting();
};


#endif //GDDLINTEGRATION_SEARCHOBJECT_H