#include "TextSearchSetting.h"

#include <Utils.h>

TextSearchSetting::TextSearchSetting(const std::string& settingKey, const std::string& defaultValue, const unsigned maxLength): SearchSetting(settingKey, defaultValue), maxLength(maxLength) {}

void TextSearchSetting::setSettingValue(const std::string value) {
    if (maxLength != -1) {
        this->value = value.substr(0, maxLength);
    }
}

void TextSearchSetting::loadSetting() {
    SearchSetting::loadSetting();
    setSettingValue(value);
}

std::string TextSearchSetting::getSearchQueryFragment() {
    if (this->value != this->defaultValue) {
        return "&" + this->searchQueryParameterName + "=" + Utils::urlEncode(this->value);
    }
    return "";
}
