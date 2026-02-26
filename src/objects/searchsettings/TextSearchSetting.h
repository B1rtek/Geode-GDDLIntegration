#ifndef GDDLINTEGRATION_TEXTSEARCHSETTING_H
#define GDDLINTEGRATION_TEXTSEARCHSETTING_H

#include "SearchSetting.h"

class TextSearchSetting : public SearchSetting<std::string> {
protected:
    const unsigned maxLength;
public:
    explicit TextSearchSetting(const std::string& settingKey, const std::string& defaultValue = "", unsigned maxLength = -1);

    void setSettingValue(std::string value) override;
    void loadSetting() override;
    std::string getSearchQueryFragment() override;
};


#endif //GDDLINTEGRATION_TEXTSEARCHSETTING_H