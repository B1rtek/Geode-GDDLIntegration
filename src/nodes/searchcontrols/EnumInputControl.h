#ifndef GDDLINTEGRATION_ENUMINPUTCONTROL_H
#define GDDLINTEGRATION_ENUMINPUTCONTROL_H

#include <objects/searchsettings/EnumSearchSetting.h>
#include "SearchInputControl.h"

class EnumInputControl : public SearchInputControl {
    CCLabelBMFont* enumLabel = nullptr;

    std::shared_ptr<EnumSearchSetting> relatedSetting;

    bool init(const std::string& labelText, std::shared_ptr<EnumSearchSetting> relatedSetting);

    void onEnumValueLeft(CCObject *sender);
    void onEnumValueRight(CCObject *sender);
public:
    static EnumInputControl *create(const std::string& labelText, std::shared_ptr<EnumSearchSetting> relatedSetting);
};


#endif //GDDLINTEGRATION_ENUMINPUTCONTROL_H