#ifndef GDDLINTEGRATION_INTRANGEINPUTCONTROL_H
#define GDDLINTEGRATION_INTRANGEINPUTCONTROL_H

#include <objects/searchsettings/RangeSearchSetting.h>
#include "SearchInputControl.h"

template<typename T>
class RangeInputControl : public SearchInputControl {
    TextInput* minTextInput = nullptr;
    TextInput* maxTextInput = nullptr;
    std::shared_ptr<RangeSearchSetting<T>> relatedSetting;
    bool placeholderForMinValue = false;
    std::string placeholderContent;

    bool init(const std::string& labelText, const std::shared_ptr<RangeSearchSetting<T>>& relatedSetting,
              bool placeholderForMinValue = false, std::string placeholderContent = "") {
        
    }

    void onMinValueLeft(CCObject* sender) {
        
    }

    void onMinValueRight(CCObject* sender) {
        
    }

    void onMaxValueLeft(CCObject* sender) {
        
    }

    void onMaxValueRight(CCObject* sender) {
        
    }

public:
    static RangeInputControl* create(const std::string& labelText,
                                     const std::shared_ptr<RangeSearchSetting<T>>& relatedSetting,
                                     bool placeholderForMinValue = false, std::string placeholderContent = "") {
        
    }

    void saveSetting() override {
        SearchInputControl::saveSetting();
    }
};


#endif //GDDLINTEGRATION_INTRANGEINPUTCONTROL_H