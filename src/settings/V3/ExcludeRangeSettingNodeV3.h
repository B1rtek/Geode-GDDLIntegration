#ifndef EXCLUDERANGESETTINGNODEV3_H
#define EXCLUDERANGESETTINGNODEV3_H

#include <Geode/loader/SettingV3.hpp>

#include "ExcludeRangeSettingV3.h"

class ExcludeRangeSettingNodeV3 : public SettingNodeV3 {
protected:
    bool currentInclude = ExcludeRangeSettingV3::defaultInclude;
    int currentRangeBegin = ExcludeRangeSettingV3::defaultRangeBegin;
    int currentRangeEnd = ExcludeRangeSettingV3::defaultRangeEnd;
    bool dontTouchTheToggler = false; // broken updateState() workaround

    std::vector<CCTextInputNode*> textfields = {nullptr, nullptr};
    CCMenuItemToggler* includeToggler = nullptr;

    bool init(std::shared_ptr<ExcludeRangeSettingV3> setting, float width);
    void loadValues();
    void updateState(CCNode* invoker) override;

    void onRangeBeginLeft(CCObject *sender);
    void onRangeBeginRight(CCObject *sender);
    void onRangeEndLeft(CCObject *sender);
    void onRangeEndRight(CCObject *sender);
    void onToggleInclude(CCObject *sender);

    void onCommit() override;
    void onResetToDefault() override;

public:
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    std::shared_ptr<ExcludeRangeSettingV3> getSetting() const;

    static ExcludeRangeSettingNodeV3* create(std::shared_ptr<ExcludeRangeSettingV3> setting, float width);

};



#endif //EXCLUDERANGESETTINGNODEV3_H
