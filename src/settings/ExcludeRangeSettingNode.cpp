#include "ExcludeRangeSettingNode.h"

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/cocos.hpp>

#include "Utils.h"

bool ExcludeRangeSettingNode::init(ExcludeRangeSetting *value, float width) {
    if (!SettingNode::init(value)) {
        return false;
    }
    this->setContentSize({width, 50.f});

    const auto menu = CCMenu::create();
    menu->setContentSize(this->getContentSize());

    // name and info button
    Utils::createLabel(menu, "bigFont.fnt", "Exclude range", 100.0f, {70.0f, 25.0f});

    const auto infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoButtonSprite->setScale(0.6f);
    const auto infoButton = CCMenuItemSpriteExtra::create(infoButtonSprite, this, menu_selector(ExcludeRangeSettingNode::onInfo));
    menu->addChild(infoButton);
    infoButton->setPosition({135.0f, 25.0f});

    // range textboxes
    Utils::createTextInputNode(menu, textfields[0], "bigFont.fnt", "", 2, {35.0f, 25.0f},
                               {185.0f, 25.0f});
    Utils::createLeftRightButtonsAround(textfields[0], {13.0f, 19.0f}, this,
                                        menu_selector(ExcludeRangeSettingNode::onRangeBeginLeft),
                                        menu_selector(ExcludeRangeSettingNode::onRangeBeginRight));
    Utils::createTextInputNode(menu, textfields[1], "bigFont.fnt", "", 2, {35.0f, 25.0f},
                               {260.0f, 25.0f});
    Utils::createLeftRightButtonsAround(textfields[1], {13.0f, 19.0f}, this,
                                        menu_selector(ExcludeRangeSettingNode::onRangeEndLeft),
                                        menu_selector(ExcludeRangeSettingNode::onRangeEndRight));
    Utils::createLabel(menu, "chatFont.fnt", "to", 30.0f, {222.5f, 25.0f});

    // include/exclude toggle
    Utils::createCheckbox(menu, includeToggler, "Include", 17.5f, 0.6f, {315.0f, 30.0f}, this,
                          menu_selector(ExcludeRangeSettingNode::onToggleInclude));
    this->addChild(menu);
    this->updateLayout();
    menu->setPosition({0.0f, 0.0f});

    // load values
    loadValues();

    cocos::handleTouchPriority(this);
    return true;
}



void ExcludeRangeSettingNode::loadValues() {
    const auto settingsObject = dynamic_cast<ExcludeRangeSetting *>(m_value);
    Utils::setNumberWithDefZeroTextfield(settingsObject->getRangeBegin(), textfields[0]);
    Utils::setNumberWithDefZeroTextfield(settingsObject->getRangeEnd(), textfields[1]);
    includeToggler->toggle(settingsObject->isInclude());
    currentInclude = settingsObject->isInclude();
}

void ExcludeRangeSettingNode::onRangeBeginLeft(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(textfields[0]) - 1;
    if (newValue < 0)
        newValue = highestTier;
    Utils::setNumberWithDefZeroTextfield(newValue, textfields[0]);
    this->dispatchChanged();
}

void ExcludeRangeSettingNode::onRangeBeginRight(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(textfields[0]) + 1;
    if (newValue > highestTier)
        newValue = 0;
    Utils::setNumberWithDefZeroTextfield(newValue, textfields[0]);
    this->dispatchChanged();
}

void ExcludeRangeSettingNode::onRangeEndLeft(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(textfields[1]) - 1;
    if (newValue < 0)
        newValue = highestTier;
    Utils::setNumberWithDefZeroTextfield(newValue, textfields[1]);
    this->dispatchChanged();
}

void ExcludeRangeSettingNode::onRangeEndRight(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(textfields[1]) + 1;
    if (newValue > highestTier)
        newValue = 0;
    Utils::setNumberWithDefZeroTextfield(newValue, textfields[1]);
    this->dispatchChanged();
}

void ExcludeRangeSettingNode::onToggleInclude(CCObject *sender) {
    currentInclude = !includeToggler->isOn();
    this->dispatchChanged();
}

void ExcludeRangeSettingNode::onInfo(CCObject *sender) {
    FLAlertLayer::create("Exclude range", "<cr>Removes</c> the GDDL button from level pages of levels <cy>within</c> the specified range. If the <cb>include</c> toggle is toggled <cg>on</c>, the ratings will <cy>only</c> be displayed in the specified range. If both range ends are set to <cb>0</c>, this setting does <cy>nothing</c>, an empty value means 1 (range beginning) and 35 (range end) by default. Range is <cg>inclusive</c> on <cb>both</c> ends.", "OK")->show();
}

void ExcludeRangeSettingNode::commit() {
    const auto settingsObject = dynamic_cast<ExcludeRangeSetting *>(m_value);
    settingsObject->setInclude(currentInclude);
    settingsObject->setRangeBegin(Utils::getNumberTextfieldValue(textfields[0]));
    settingsObject->setRangeEnd(Utils::getNumberTextfieldValue(textfields[1]));
    this->dispatchCommitted();
}

bool ExcludeRangeSettingNode::hasUncommittedChanges() {
    const auto settingsObject = dynamic_cast<ExcludeRangeSetting *>(m_value);
    return settingsObject->getRangeBegin() != Utils::getNumberTextfieldValue(textfields[0]) || settingsObject->
           getRangeEnd() != Utils::getNumberTextfieldValue(textfields[1]) || settingsObject->isInclude() !=
           currentInclude;
}

bool ExcludeRangeSettingNode::hasNonDefaultValue() {
    return Utils::getNumberTextfieldValue(textfields[0]) != 0 || Utils::getNumberTextfieldValue(textfields[1]) != 0 || !currentInclude;
}

void ExcludeRangeSettingNode::resetToDefault() {
    Utils::setNumberWithDefZeroTextfield(0, textfields[0]);
    Utils::setNumberWithDefZeroTextfield(0, textfields[1]);
    includeToggler->toggle(false);
    currentInclude = false;
    this->dispatchChanged();
}

ExcludeRangeSettingNode * ExcludeRangeSettingNode::create(ExcludeRangeSetting *value, float width) {
    auto ret = new ExcludeRangeSettingNode;
    if (ret && ret->init(value, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


