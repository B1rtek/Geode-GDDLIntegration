#include "SearchObject.h"
void SearchObject::loadSettings() {
    levelNameSetting->loadSetting();
}

void SearchObject::saveSettings() {
    levelNameSetting->saveSetting();
}

std::shared_ptr<TextSearchSetting> SearchObject::getLevelNameSetting() {
    return this->levelNameSetting;
}
