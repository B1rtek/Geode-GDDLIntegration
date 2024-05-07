#include "UseOldTierLabelSettingNode.h"

#include <Utils.h>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/utils/cocos.hpp>

#include "ExcludeRangeSettingNode.h"

bool UseOldTierLabelSettingNode::init(UseOldTierLabelSetting *value, float width) {
    if (!SettingNode::init(value)) {
        return false;
    }
    this->setContentSize({width, 50.f});

    const auto menu = CCMenu::create();
    menu->setContentSize(this->getContentSize());

    // name and info button
    Utils::createLabel(menu, "bigFont.fnt", "Use old tier label", 100.0f, {70.0f, 25.0f});

    const auto infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoButtonSprite->setScale(0.6f);
    const auto infoButton = CCMenuItemSpriteExtra::create(infoButtonSprite, this,
                                                          menu_selector(UseOldTierLabelSettingNode::onInfo));
    menu->addChild(infoButton);
    infoButton->setPosition({135.0f, 25.0f});

    // setting UI
    Utils::createCheckbox(menu, enabledToggler, "Enable", 17.5f, 0.6f, {196.0f, 30.0f}, this,
                          menu_selector(UseOldTierLabelSettingNode::onToggleEnabled));
    Utils::createTextInputNode(menu, positionOffsetTextfield, "bigFont.fnt", "", 2, {35.0f, 25.0f},
                               {281.0f, 25.0f});
    Utils::createLeftRightButtonsAround(positionOffsetTextfield, {13.0f, 19.0f}, this,
                                        menu_selector(UseOldTierLabelSettingNode::onPositionOffsetLeft),
                                        menu_selector(UseOldTierLabelSettingNode::onPositionOffsetRight));
    Utils::createLabel(menu, "chatFont.fnt", "Offset:", 50.0f, {231.0f, 25.0f});

    this->addChild(menu);
    this->updateLayout();
    menu->setPosition({0.0f, 0.0f});

    // load values
    loadValues();

    cocos::handleTouchPriority(this);
    return true;
}

void UseOldTierLabelSettingNode::loadValues() {
    const auto settingsObject = dynamic_cast<UseOldTierLabelSetting *>(m_value);
    positionOffsetTextfield->setString(std::to_string(settingsObject->getPositionOffset()).c_str());
    enabledToggler->toggle(settingsObject->isEnabled());
    currentEnabled = settingsObject->isEnabled();
}

void UseOldTierLabelSettingNode::onPositionOffsetLeft(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(positionOffsetTextfield) - 1;
    if (newValue < minOffset) {
        newValue = maxOffset;
    }
    positionOffsetTextfield->setString(std::to_string(newValue).c_str());
    this->dispatchChanged();
}

void UseOldTierLabelSettingNode::onPositionOffsetRight(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(positionOffsetTextfield) + 1;
    if (newValue > maxOffset) {
        newValue = minOffset;
    }
    positionOffsetTextfield->setString(std::to_string(newValue).c_str());
    this->dispatchChanged();
}

void UseOldTierLabelSettingNode::onToggleEnabled(CCObject *sender) {
    currentEnabled = !enabledToggler->isOn();
    this->dispatchChanged();
}

void UseOldTierLabelSettingNode::onInfo(CCObject *sender) {
    FLAlertLayer::create("Use old tier label",
                         "<cy>Changes</c> the <cr>tier button</c> on the <cb>level info page</c> into a <cg>clickable label</c>, just like it used to be in the <co>early versions of this mod</c>. Allows you to <cy>specify an offset</c> to move the label <cg>up</c> <cb>or</c> <cr>down</c> by the given number of rows. <cr>This settings overrides \"Button position\"!</c>",
                         "OK")->show();
}

void UseOldTierLabelSettingNode::commit() {
    const auto settingsObject = dynamic_cast<UseOldTierLabelSetting *>(m_value);
    settingsObject->setEnabled(currentEnabled);
    settingsObject->setPositionOffset(Utils::getNumberTextfieldValue(positionOffsetTextfield));
    this->dispatchCommitted();
}

bool UseOldTierLabelSettingNode::hasUncommittedChanges() {
    const auto settingsObject = dynamic_cast<UseOldTierLabelSetting *>(m_value);
    return settingsObject->isEnabled() != currentEnabled || settingsObject->getPositionOffset() !=
           Utils::getNumberTextfieldValue(positionOffsetTextfield);
}

bool UseOldTierLabelSettingNode::hasNonDefaultValue() {
    return currentEnabled || Utils::getNumberTextfieldValue(positionOffsetTextfield) != 0;
}

void UseOldTierLabelSettingNode::resetToDefault() {
    enabledToggler->toggle(false);
    currentEnabled = false;
    positionOffsetTextfield->setString("0");
    this->dispatchChanged();
}

UseOldTierLabelSettingNode *UseOldTierLabelSettingNode::create(UseOldTierLabelSetting *value, float width) {
    auto ret = new UseOldTierLabelSettingNode;
    if (ret && ret->init(value, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
