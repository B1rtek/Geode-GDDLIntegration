#ifndef GDDLINTEGRATION_BOOLSEARCHSETTING_H
#define GDDLINTEGRATION_BOOLSEARCHSETTING_H

#include "SearchSetting.h"

class BoolSearchSetting : public SearchSetting<bool> {
public:
    explicit BoolSearchSetting(const std::string& settingKeyBase, bool defaultValue = false) : SearchSetting(settingKeyBase, defaultValue) {}

    std::string getSearchQueryFragment() override {
        if (this->value != this->defaultValue) {
            return "&" + this->searchQueryParameterName + "=" + (this->value ? "true" : "false");
        }
        return "";
    }
};


#endif //GDDLINTEGRATION_BOOLSEARCHSETTING_H