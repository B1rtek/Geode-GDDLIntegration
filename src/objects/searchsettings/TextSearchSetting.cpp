#include "TextSearchSetting.h"

TextSearchSetting::TextSearchSetting(const std::string& settingKey, const unsigned maxLength): SearchSetting(settingKey), maxLength(maxLength) {
    TextSearchSetting::getSavedSetting();
}

void TextSearchSetting::setSettingValue(const std::string value) {
    if (maxLength != -1) {
        this->value = value.substr(0, maxLength);
    }
}

void TextSearchSetting::getSavedSetting() {
    SearchSetting::getSavedSetting();
    setSettingValue(value);
}
