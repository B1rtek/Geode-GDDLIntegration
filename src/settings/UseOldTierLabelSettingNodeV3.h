#ifndef GDDLINTEGRATION_USEOLDTIERLABELSETTINGNODEV3_H
#define GDDLINTEGRATION_USEOLDTIERLABELSETTINGNODEV3_H

#include <Geode/loader/SettingV3.hpp>

#include "UseOldTierLabelSettingV3.h"

class UseOldTierLabelSettingNodeV3 : public SettingNodeV3 {
protected:
    bool currentEnabled = UseOldTierLabelSettingV3::defaultEnabled;
    int currentPositionOffset = UseOldTierLabelSettingV3::defaultPositionOffset;
    bool dontTouchTheToggler = false; // fun fact: updateState() breaks toggler animation (at least as of 3.8.1)

    CCTextInputNode* positionOffsetTextfield = nullptr;
    CCMenuItemToggler* enabledToggler = nullptr;

    bool init(std::shared_ptr<UseOldTierLabelSettingV3> setting, float width);
    void loadValues();
    void updateState(CCNode* invoker) override;

    void onPositionOffsetLeft(CCObject *sender);
    void onPositionOffsetRight(CCObject *sender);
    void onToggleEnabled(CCObject *sender);

    void onCommit() override;
    void onResetToDefault() override;

public:
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    std::shared_ptr<UseOldTierLabelSettingV3> getSetting() const;

    static UseOldTierLabelSettingNodeV3* create(std::shared_ptr<UseOldTierLabelSettingV3> setting, float width);
};


#endif //GDDLINTEGRATION_USEOLDTIERLABELSETTINGNODEV3_H
