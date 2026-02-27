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
    for (const auto& setting : searchableSettings) {
        request += setting->getSearchQueryFragment();
    }
    log::info("Search request: {}", request);
    return request;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getSortSetting() {
    return this->sortSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getSortDirectionSetting() {
    return this->sortDirectionSetting;
}

std::shared_ptr<TextSearchSetting> SearchObject::getLevelNameSetting() {
    return this->levelNameSetting;
}

std::shared_ptr<TextSearchSetting> SearchObject::getCreatorNameSetting() {
    return this->creatorNameSetting;
}

std::shared_ptr<TextSearchSetting> SearchObject::getSongNameSetting() {
    return this->songNameSetting;
}

std::shared_ptr<RangeSearchSetting<int>> SearchObject::getRatingsSetting() {
    return this->ratingsSetting;
}

std::shared_ptr<RangeSearchSetting<float>> SearchObject::getEnjoymentsSetting() {
    return this->enjoymentsSetting;
}

std::shared_ptr<RangeSearchSetting<int>> SearchObject::getEnjoymentsCountSetting() {
    return this->enjoymentsCountSetting;
}

std::shared_ptr<RangeSearchSetting<float>> SearchObject::getDeviationsSetting() {
    return this->deviationsSetting;
}

std::shared_ptr<RangeSearchSetting<int>> SearchObject::getRatingsCountSetting() {
    return this->ratingsCountSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getDifficultySetting() {
    return this->difficultySetting;
}

std::shared_ptr<RangeSearchSetting<int>> SearchObject::getIdsRangeSetting() {
    return this->idsRangeSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getLengthSetting() {
    return this->lengthSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getTwoPlayerSetting() {
    return this->twoPlayerSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getIsInPackSetting() {
    return this->isInPackSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getTopSkillsetSetting() {
    return this->topSkillsetSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getHasSkillsetSetting() {
    return this->hasSkillsetSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getCompletedSetting() {
    return this->completedSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getUncompletedSetting() {
    return this->uncompletedSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getExcludeRatedSetting() {
    return this->excludeRatedSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getExcludeRatedEnjoymentSetting() {
    return this->excludeRatedEnjoymentSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getExcludeUnratedSetting() {
    return this->excludeUnratedSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getExcludeUnratedEnjoymentSetting() {
    return this->excludeUnratedEnjoymentSetting;
}
