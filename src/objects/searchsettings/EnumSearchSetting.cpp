#include "EnumSearchSetting.h"

EnumSearchSetting::EnumSearchSetting(const std::string& settingKey, const std::vector<std::string>& options,
    int defaultValue): SearchSetting(settingKey, defaultValue), options(options) {}

void EnumSearchSetting::setSettingValue(int value) {
    if (value < 0) {
        this->value = value % options.size() == 0 ? 0 : options.size() + value % static_cast<int>(options.size());
    } else {
        this->value = value % options.size();
    }
}

void EnumSearchSetting::loadSetting() {
    SearchSetting::loadSetting();
    setSettingValue(value);
}

std::string EnumSearchSetting::getEnumValue() const {
    return options[value];
}
