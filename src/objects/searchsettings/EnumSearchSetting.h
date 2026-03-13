#ifndef GDDLINTEGRATION_ENUMSEARCHSETTING_H
#define GDDLINTEGRATION_ENUMSEARCHSETTING_H

#include "SearchSetting.h"

class EnumSearchSetting : public SearchSetting<int> {
protected:
    const std::vector<std::string> options, queryStrings;
    int queryEnumOffset = 0;
public:
    EnumSearchSetting(const std::string& settingKey, const std::vector<std::string>& options, int defaultValue = 0, int queryEnumOffset = 0, const std::vector<std::string>& queryStrings = {});

    void setSettingValue(int value) override;
    void loadSetting() override;
    std::string getEnumValue() const;
    std::string getSearchQueryFragment() override;
};

#endif //GDDLINTEGRATION_ENUMSEARCHSETTING_H
