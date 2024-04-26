#include "ExcludeRangeSettingNode.h"

#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/loader/Log.hpp>

#include "Utils.h"

bool ExcludeRangeSettingNode::init(ExcludeRangeSetting *value, float width) {
    log::info("ExcludeRangeSettingNode::init called");
    if (!SettingNode::init(value)) {
        return false;
    }
    log::info("SettingNode::init returned true");
    this->setContentSize({width, 40.f});

    const auto menu = CCMenu::create();
    menu->setContentSize(this->getContentSize());
    menu->setPosition(width / 2, 20.f);

    float xPosition = 50.0f;
    for (auto &textfield: textfields) {
        Utils::createTextInputNode(menu, textfield, "bigFont.fnt", "", 2, {35.0f, 25.0f},
                                   {xPosition, 20.0f});
        Utils::createLeftRightButtonsAround(textfield, {13.0f, 19.0f}, this,
                                            menu_selector(ExcludeRangeSettingNode::onRangeBeginLeft),
                                            menu_selector(ExcludeRangeSettingNode::onRangeBeginRight));
        xPosition += 50.0f;
    }

    Utils::createCheckbox(menu, includeToggler, "Include", 17.5f, 0.9f, {xPosition, 20.0f}, this,
                          menu_selector(ExcludeRangeSettingNode::onToggleInclude));
    log::info("ExcludeRangeSettingNode::init finished");
    this->addChild(menu);
    this->updateLayout();
    return true;
}

void ExcludeRangeSettingNode::onRangeBeginLeft(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(textfields[0]) + 1;
    if (newValue > highestTier)
        newValue = 0;
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
    int newValue = Utils::getNumberTextfieldValue(textfields[1]) + 1;
    if (newValue > highestTier)
        newValue = 0;
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
    this->dispatchChanged();
}

void ExcludeRangeSettingNode::commit() {
    log::info("ExcludeRangeSettingNode::commit called");
    const auto settingsObject = dynamic_cast<ExcludeRangeSetting *>(m_value);
    settingsObject->setInclude(includeToggler->isToggled());
    settingsObject->setRangeBegin(Utils::getNumberTextfieldValue(textfields[0]));
    settingsObject->setRangeEnd(Utils::getNumberTextfieldValue(textfields[1]));
    this->dispatchCommitted();
}

bool ExcludeRangeSettingNode::hasUncommittedChanges() {
    log::info("ExcludeRangeSettingNode::hasUncommittedChanges called");
    const auto settingsObject = dynamic_cast<ExcludeRangeSetting *>(m_value);
    return settingsObject->getRangeBegin() != Utils::getNumberTextfieldValue(textfields[0]) || settingsObject->
           getRangeEnd() != Utils::getNumberTextfieldValue(textfields[1]) || settingsObject->isInclude() !=
           includeToggler->isToggled();
}

bool ExcludeRangeSettingNode::hasNonDefaultValue() {
    log::info("ExcludeRangeSettingNode::hasNonDefaultValue called");
    return Utils::getNumberTextfieldValue(textfields[0]) != 0 || Utils::getNumberTextfieldValue(textfields[1]) != 0 || !includeToggler->isToggled();
}

void ExcludeRangeSettingNode::resetToDefault() {
    log::info("ExcludeRangeSettingNode::resetToDefault called");
    Utils::setNumberWithDefZeroTextfield(0, textfields[0]);
    Utils::setNumberWithDefZeroTextfield(0, textfields[1]);
    includeToggler->toggle(false);
}

ExcludeRangeSettingNode * ExcludeRangeSettingNode::create(ExcludeRangeSetting *value, float width) {
    log::info("ExcludeRangeSettingNode::create called");
    auto ret = new ExcludeRangeSettingNode;
    if (ret && ret->init(value, width)) {
        log::info("ExcludeRangeSettingNode::create init returned true");
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


