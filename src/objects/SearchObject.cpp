#include "SearchObject.h"
void SearchObject::loadSettings() {
    for (const auto& setting : settings) {
        setting->loadSetting();
    }
}

void SearchObject::saveSettings() {
    for (const auto& setting : settings) {
        setting->saveSetting();
    }
}

void SearchObject::resetToDefaults() {
    for (const auto& setting : settings) {
        setting->resetToDefault();
    }
}

std::string SearchObject::createSearchQuery() {
    std::string request = searchEndpoint;
    request += "?page=" + std::to_string(apiPagesFetched) + "&limit=25";
    for (const auto& setting : settings) {
        request += setting->getSearchQueryFragment();
    }
    log::info("Search request: {}", request);
    return request;
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

std::shared_ptr<BoolSearchSetting> SearchObject::getExactNameSetting() {
    return this->exactNameSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getRemoveUnratedSetting() {
    return this->removeUnratedSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getRemoveRatedSetting() {
    return this->removeRatedSetting;
}
