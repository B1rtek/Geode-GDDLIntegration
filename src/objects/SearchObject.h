#ifndef GDDLINTEGRATION_SEARCHOBJECT_H
#define GDDLINTEGRATION_SEARCHOBJECT_H

#include <memory>

#include "searchsettings/EnumSearchSetting.h"
#include "searchsettings/TextSearchSetting.h"

class SearchObject {
    std::shared_ptr<TextSearchSetting> levelNameSetting = std::make_shared<TextSearchSetting>("search-name", "", 32);
    std::shared_ptr<EnumSearchSetting> difficultySetting = std::make_shared<EnumSearchSetting>("search-difficulty", std::vector<std::string>{"Easy Demon", "Medium Demon", "Hard Demon", "Insane Demon", "Extreme Demon", "Any"}, 5);

public:
    SearchObject() = default;

    void loadSettings();
    void saveSettings();

    std::shared_ptr<TextSearchSetting> getLevelNameSetting();
    std::shared_ptr<EnumSearchSetting> getDifficultySetting();
};


#endif //GDDLINTEGRATION_SEARCHOBJECT_H