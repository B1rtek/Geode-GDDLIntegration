#include "SearchObject.h"
void SearchObject::loadSettings() {
    for (const auto& setting: settings) {
        setting->loadSetting();
    }
}

void SearchObject::saveSettings() {
    for (const auto& setting: settings) {
        setting->saveSetting();
    }
}

void SearchObject::resetToDefaults() {
    for (const auto& setting: settings) {
        setting->resetToDefault();
    }
}

std::string SearchObject::createSearchQuery() {
    return "";
}

std::shared_ptr<TextSearchSetting> SearchObject::getLevelNameSetting() {
    return this->levelNameSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getDifficultySetting() {
    return this->difficultySetting;
}

std::shared_ptr<RangeSearchSetting<int>> SearchObject::getTiersSetting() {
    return this->tiersSetting;
}

std::shared_ptr<RangeSearchSetting<float>> SearchObject::getEnjoymentsSetting() {
    return this->enjoymentsSetting;
}

std::shared_ptr<SearchSetting<bool>> SearchObject::getExactNameSetting() {
    return this->exactNameSetting;
}

std::shared_ptr<SearchSetting<bool>> SearchObject::getRemoveUnratedSetting() {
    return this->removeUnratedSetting;
}

std::shared_ptr<SearchSetting<bool>> SearchObject::getRemoveRatedSetting() {
    return this->removeRatedSetting;
}
