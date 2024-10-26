#include "ButtonPositionSettingNodeV3.h"
#include "Utils.h"

bool ButtonPositionSettingNodeV3::init(std::shared_ptr<ButtonPositionSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width)) {
        return false;
    }

    // choice label
    const auto bg = Utils::createLabelForChoice(this->getButtonMenu(), positionSettingLabel, "bigFont.fnt", toDisplay[0], labelWidth, {100.0f, 15.0f},
                                   {labelWidth, 25.0f});
    Utils::createLeftRightButtonsAround(bg, {13.0f, 19.0f}, this, menu_selector(ButtonPositionSettingNodeV3::onPositionSettingLeft), menu_selector(ButtonPositionSettingNodeV3::onPositionSettingRight));

    this->getButtonMenu()->setContentWidth(bg->getScaledContentWidth() * 1.3f);
    this->getButtonMenu()->updateLayout();

    // load values
    loadValues();
    this->updateState(nullptr);

    return true;
}

void ButtonPositionSettingNodeV3::loadValues() {
    currentSetting = this->getSetting()->getPosition();
}


void ButtonPositionSettingNodeV3::updateState(CCNode* invoker) {
    SettingNodeV3::updateState(invoker);
    setPositionSettingLabel();
}

void ButtonPositionSettingNodeV3::setPositionSettingLabel() {
    positionSettingLabel->setString(toDisplay[currentSetting].c_str());
    Utils::scaleLabelToWidth(positionSettingLabel, labelWidth);
}

void ButtonPositionSettingNodeV3::onPositionSettingLeft(CCObject* sender) {
    int optionIndex = currentSetting;
    --optionIndex;
    if (optionIndex < 0) {
        optionIndex = toDisplay.size() - 1;
    }
    currentSetting = static_cast<ButtonPosition>(optionIndex);
    this->markChanged(nullptr);
}

void ButtonPositionSettingNodeV3::onPositionSettingRight(CCObject* sender) {
    int optionIndex = currentSetting;
    ++optionIndex;
    if (optionIndex > toDisplay.size() - 1) {
        optionIndex = 0;
    }
    currentSetting = static_cast<ButtonPosition>(optionIndex);
    this->markChanged(nullptr);
}

void ButtonPositionSettingNodeV3::onCommit() {
    this->getSetting()->setPosition(currentSetting);
}

void ButtonPositionSettingNodeV3::onResetToDefault() {
    currentSetting = ButtonPositionSettingV3::defaultPosition;
    this->markChanged(nullptr);
}

bool ButtonPositionSettingNodeV3::hasUncommittedChanges() const {
    return this->getSetting()->getPosition() != currentSetting;
}

bool ButtonPositionSettingNodeV3::hasNonDefaultValue() const {
    return !this->getSetting()->isDefaultValue();
}

std::shared_ptr<ButtonPositionSettingV3> ButtonPositionSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<ButtonPositionSettingV3>(SettingNodeV3::getSetting());
}

ButtonPositionSettingNodeV3* ButtonPositionSettingNodeV3::create(std::shared_ptr<ButtonPositionSettingV3> setting,
    float width) {
    auto ret = new ButtonPositionSettingNodeV3();
    if(ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
