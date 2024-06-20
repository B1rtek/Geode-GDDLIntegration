#ifndef EXCLUDERANGESETTINGNODE_H
#define EXCLUDERANGESETTINGNODE_H

#include <Geode/binding/CCTextInputNode.hpp>

#include "ExcludeRangeSetting.h"
#include <Geode/loader/SettingNode.hpp>

using namespace geode::prelude;

class ExcludeRangeSettingNode : public SettingNode {
protected:
    bool currentInclude = false; // stupid f***ing toggler

    std::vector<CCTextInputNode*> textfields = {nullptr, nullptr};
    CCMenuItemToggler* includeToggler;

    bool init(ExcludeRangeSetting* value, float width);
    void loadValues();

    void onRangeBeginLeft(CCObject *sender);
    void onRangeBeginRight(CCObject *sender);
    void onRangeEndLeft(CCObject *sender);
    void onRangeEndRight(CCObject *sender);
    void onToggleInclude(CCObject *sender);
    void onInfo(CCObject *sender);

public:
    static constexpr int highestTier = 35;

    void commit() override;

    bool hasUncommittedChanges() override;

    bool hasNonDefaultValue() override;

    void resetToDefault() override;

    static ExcludeRangeSettingNode* create(ExcludeRangeSetting* value, float width);
};



#endif //EXCLUDERANGESETTINGNODE_H
