#ifndef GDDLINTEGRATION_CHECKBOXINPUTCONTROL_H
#define GDDLINTEGRATION_CHECKBOXINPUTCONTROL_H
#include <objects/searchsettings/SearchSetting.h>

#include "SearchInputControl.h"


class CheckboxInputControl : public SearchInputControl {
    CCMenuItemToggler* settingToggle1 = nullptr;
    CCMenuItemToggler* settingToggle2 = nullptr;
    std::shared_ptr<SearchSetting<bool>> relatedSetting1 = nullptr;
    std::shared_ptr<SearchSetting<bool>> relatedSetting2 = nullptr;
    bool mutuallyExclusive = false;

    bool init(const std::string& labelText1, const std::shared_ptr<SearchSetting<bool>> relatedSetting1,
              const std::string& labelText2 = "", const std::shared_ptr<SearchSetting<bool>> relatedSetting2 = nullptr,
              bool mutuallyExclusive = false);

    void updateValues(unsigned updateSource);
    void onToggler1Toggled(CCObject* sender);
    void onToggler2Toggled(CCObject* sender);

public:
    static CheckboxInputControl* create(const std::string& labelText,
                                        std::shared_ptr<SearchSetting<bool>> relatedSetting1,
                                        const std::string& labelText2 = "",
                                        std::shared_ptr<SearchSetting<bool>> relatedSetting2 = nullptr,
                                        bool mutuallyExclusive = false);

    void saveSetting() override;
};


#endif //GDDLINTEGRATION_CHECKBOXINPUTCONTROL_H
