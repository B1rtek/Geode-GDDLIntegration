#ifndef BUTTONPOSITIONSETTINGNODEV3_H
#define BUTTONPOSITIONSETTINGNODEV3_H

#include <Geode/loader/SettingV3.hpp>

#include "ButtonPositionSettingV3.h"

class ButtonPositionSettingNodeV3 : public SettingNodeV3 {
protected:
    ButtonPosition currentSetting = DEFAULT;
    const inline static std::vector<std::string> toDisplay = {"Default", "Left of the level title", "Right of the level title", "Demon face as button"};

    CCLabelBMFont *positionSettingLabel = nullptr;
    static constexpr float labelWidth = 150.0f;

    bool init(std::shared_ptr<ButtonPositionSettingV3> setting, float width);
    void loadValues();
    void updateState(CCNode* invoker) override;
    void setPositionSettingLabel();

    void onPositionSettingLeft(CCObject *sender);
    void onPositionSettingRight(CCObject *sender);

    void onCommit() override;
    void onResetToDefault() override;

public:

    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    std::shared_ptr<ButtonPositionSettingV3> getSetting() const;

    static ButtonPositionSettingNodeV3* create(std::shared_ptr<ButtonPositionSettingV3> setting, float width);
};



#endif //BUTTONPOSITIONSETTINGNODEV3_H
