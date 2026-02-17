#include "SearchObject.h"
void SearchObject::saveToSaved() {
    levelNameSetting->saveSetting();
}

std::shared_ptr<TextSearchSetting> SearchObject::getLevelNameSetting() {
    return this->levelNameSetting;
}
