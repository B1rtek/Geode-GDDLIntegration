#ifndef USEOLDTIERLABELSETTINGNODE_H
#define USEOLDTIERLABELSETTINGNODE_H

#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/loader/SettingNode.hpp>

#include "UseOldTierLabelSetting.h"

using namespace geode::prelude;

class UseOldTierLabelSettingNode : public SettingNode {
protected:
    static constexpr int minOffset = -1, maxOffset = 3;

    bool currentEnabled = false; // stupid f***ing toggler strikes back

    CCTextInputNode* positionOffsetTextfield = nullptr;
    CCMenuItemToggler* enabledToggler = nullptr;

    bool init(UseOldTierLabelSetting* value, float width);
    void loadValues();

    void onPositionOffsetLeft(CCObject *sender);
    void onPositionOffsetRight(CCObject *sender);
    void onToggleEnabled(CCObject *sender);
    void onInfo(CCObject *sender);

public:
    void commit() override;

    bool hasUncommittedChanges() override;

    bool hasNonDefaultValue() override;

    void resetToDefault() override;

    static UseOldTierLabelSettingNode* create(UseOldTierLabelSetting* value, float width);
};



#endif //USEOLDTIERLABELSETTINGNODE_H
