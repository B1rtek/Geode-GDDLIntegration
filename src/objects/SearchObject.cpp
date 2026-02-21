#include "SearchObject.h"
void SearchObject::loadSettings() {
    levelNameSetting->loadSetting();
    difficultySetting->loadSetting();
    tiersSetting->loadSetting();
    enjoymentsSetting->loadSetting();
}

void SearchObject::saveSettings() {
    levelNameSetting->saveSetting();
    difficultySetting->saveSetting();
    tiersSetting->saveSetting();
    enjoymentsSetting->saveSetting();
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
