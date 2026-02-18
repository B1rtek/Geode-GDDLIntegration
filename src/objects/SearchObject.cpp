#include "SearchObject.h"
void SearchObject::loadSettings() {
    levelNameSetting->loadSetting();
    difficultySetting->loadSetting();
}

void SearchObject::saveSettings() {
    levelNameSetting->saveSetting();
    difficultySetting->saveSetting();
}

std::shared_ptr<TextSearchSetting> SearchObject::getLevelNameSetting() {
    return this->levelNameSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getDifficultySetting() {
    return this->difficultySetting;
}
