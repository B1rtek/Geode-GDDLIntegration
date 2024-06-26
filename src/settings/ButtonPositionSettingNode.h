#ifndef BUTTONPOSITIONSETTINGNODE_H
#define BUTTONPOSITIONSETTINGNODE_H

#include <Geode/loader/SettingNode.hpp>

#include "ButtonPositionSetting.h"

using namespace geode::prelude;

class ButtonPositionSettingNode : public SettingNode {
protected:
    ButtonPosition currentSetting = DEFAULT;

    CCLabelBMFont *positionSettingLabel = nullptr;
    static constexpr float labelWidth = 150.0f;


    bool init(ButtonPositionSetting* value, float width);
    void loadValue();
    void setPositionSettingLabel();


    void onPositionSettingLeft(CCObject *sender);
    void onPositionSettingRight(CCObject *sender);
    void onInfo(CCObject *sender);

public:
    const inline static std::vector<std::string> toDisplay = {"Default", "Left of the level title", "right of the level title"};

    void commit() override;
    bool hasUncommittedChanges() override;
    bool hasNonDefaultValue() override;
    void resetToDefault() override;
    static ButtonPositionSettingNode* create(ButtonPositionSetting* value, float width);
};



#endif //BUTTONPOSITIONSETTINGNODE_H
