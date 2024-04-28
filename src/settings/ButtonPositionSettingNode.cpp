#include "ButtonPositionSettingNode.h"

#include <Utils.h>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/utils/cocos.hpp>

#include "ExcludeRangeSettingNode.h"

bool ButtonPositionSettingNode::init(ButtonPositionSetting *value, float width) {
    if (!SettingNode::init(value)) {
        return false;
    }
    this->setContentSize({width, 40.f});

    const auto menu = CCMenu::create();
    menu->setContentSize(this->getContentSize());

    // name and info button
    Utils::createLabel(menu, "bigFont.fnt", "Button position", 100.0f, {70.0f, 20.0f});

    const auto infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoButtonSprite->setScale(0.6f);
    const auto infoButton = CCMenuItemSpriteExtra::create(infoButtonSprite, this, menu_selector(ButtonPositionSettingNode::onInfo));
    menu->addChild(infoButton);
    infoButton->setPosition({135.0f, 20.0f});

    // choice label
    const auto bg = Utils::createLabelForChoice(menu, positionSettingLabel, "bigFont.fnt", toDisplay[0], labelWidth, {245.0f, 20.0f},
                                   {labelWidth, 25.0f});
    Utils::createLeftRightButtonsAround(bg, {13.0f, 19.0f}, this, menu_selector(ButtonPositionSettingNode::onPositionSettingLeft), menu_selector(ButtonPositionSettingNode::onPositionSettingRight));

    this->addChild(menu);
    this->updateLayout();
    menu->setPosition({0.0f, 0.0f});

    // load values
    loadValue();

    cocos::handleTouchPriority(this);
    return true;
}

void ButtonPositionSettingNode::loadValue() {
    const auto settingsObject = dynamic_cast<ButtonPositionSetting *>(m_value);
    currentSetting = settingsObject->getPosition();
    setPositionSettingLabel();
}

void ButtonPositionSettingNode::setPositionSettingLabel() {
    positionSettingLabel->setString(toDisplay[currentSetting].c_str());
    Utils::scaleLabelToWidth(positionSettingLabel, labelWidth);
}

void ButtonPositionSettingNode::onPositionSettingLeft(CCObject *sender) {
    int optionIndex = currentSetting;
    --optionIndex;
    if (optionIndex < 0) {
        optionIndex = toDisplay.size() - 1;
    }
    currentSetting = static_cast<ButtonPosition>(optionIndex);
    setPositionSettingLabel();
    this->dispatchChanged();
}

void ButtonPositionSettingNode::onPositionSettingRight(CCObject *sender) {
    int optionIndex = currentSetting;
    ++optionIndex;
    if (optionIndex > toDisplay.size() - 1) {
        optionIndex = 0;
    }
    currentSetting = static_cast<ButtonPosition>(optionIndex);
    setPositionSettingLabel();
    this->dispatchChanged();
}

void ButtonPositionSettingNode::onInfo(CCObject *sender) {
    FLAlertLayer::create("Button Position", "Defines the <cr>GDDL Button</c> position on the <cb>level info</c> page. <cy>By default</c> it's positioned <cp>to the left</c> of the <cr>demon difficulty face</c>. This setting <cy>is overriden</c> by the setting \"<cb>Use old tier label</c>\"", "OK")->show();
}

void ButtonPositionSettingNode::commit() {
    const auto settingsObject = dynamic_cast<ButtonPositionSetting *>(m_value);
    settingsObject->setPosition(currentSetting);
    this->dispatchCommitted();
}

bool ButtonPositionSettingNode::hasUncommittedChanges() {
    const auto settingsObject = dynamic_cast<ButtonPositionSetting *>(m_value);
    return settingsObject->getPosition() != currentSetting;
}

bool ButtonPositionSettingNode::hasNonDefaultValue() {
    return currentSetting != DEFAULT;
}

void ButtonPositionSettingNode::resetToDefault() {
    currentSetting = DEFAULT;
    setPositionSettingLabel();
}

ButtonPositionSettingNode *ButtonPositionSettingNode::create(ButtonPositionSetting *value, float width) {
    auto ret = new ButtonPositionSettingNode;
    if (ret && ret->init(value, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
