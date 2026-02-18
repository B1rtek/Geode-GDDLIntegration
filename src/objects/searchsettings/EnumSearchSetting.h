#ifndef GDDLINTEGRATION_ENUMSEARCHSETTING_H
#define GDDLINTEGRATION_ENUMSEARCHSETTING_H

#include "SearchSetting.h"

class EnumSearchSetting : public SearchSetting<int> {
protected:
    const std::vector<std::string> options;
public:
    EnumSearchSetting(const std::string& settingKey, const std::vector<std::string>& options, int defaultValue = 0);

    void setSettingValue(int value) override;
    void loadSetting() override;
    std::string getEnumValue() const;
};

#endif //GDDLINTEGRATION_ENUMSEARCHSETTING_H
