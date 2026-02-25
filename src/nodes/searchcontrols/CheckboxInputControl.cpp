#include "CheckboxInputControl.h"

#include <Utils.h>

bool CheckboxInputControl::init(const std::string& labelText1,
                                const std::shared_ptr<SearchSetting<bool>> relatedSetting1,
                                const std::string& labelText2,
                                const std::shared_ptr<SearchSetting<bool>> relatedSetting2, bool mutuallyExclusive) {
    if (!SearchInputControl::init()) return false;

    // the actual UI of the control
    settingToggle1 = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(CheckboxInputControl::onToggler1Toggled), 0.8f);
    controlMenu->addChild(settingToggle1, 1);
    settingToggle1->setPosition({25.0f, 45.0f});
    Utils::createLabel(controlMenu, "bigFont.fnt", labelText1, standardComponentWidth - 20.0f, {85.0f, 45.0f});
    if (relatedSetting2 != nullptr) {
        settingToggle2 = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(CheckboxInputControl::onToggler2Toggled), 0.8f);
        controlMenu->addChild(settingToggle2, 1);
        settingToggle2->setPosition({25.0f, 15.0f});
        Utils::createLabel(controlMenu, "bigFont.fnt", labelText2, standardComponentWidth - 20.0f, {85.0f, 15.0f});
    }
    // associate with settings and update values
    this->relatedSetting1 = relatedSetting1;
    this->relatedSetting2 = relatedSetting2;
    this->mutuallyExclusive = mutuallyExclusive;
    this->loadSetting();
    return true;
}

void CheckboxInputControl::updateValues(unsigned updateSource) {
    // all isToggled() checks have to be flipped? idk togglers are weird
    // update settings values
    if (updateSource == 1) {
        this->relatedSetting1->setSettingValue(!this->settingToggle1->isToggled());
    } else if (relatedSetting2 != nullptr) {
        this->relatedSetting2->setSettingValue(!this->settingToggle2->isToggled());
    }
    // check mutually exclusive
    if (relatedSetting2 != nullptr && mutuallyExclusive) {
        if (updateSource == 1 && !this->settingToggle1->isToggled()) {
            this->settingToggle2->toggle(false);
            this->relatedSetting2->setSettingValue(false);
        }
        if (updateSource == 2 && !this->settingToggle2->isToggled()) {
            this->settingToggle1->toggle(false);
            this->relatedSetting1->setSettingValue(false);
        }
    }
}

void CheckboxInputControl::onToggler1Toggled(CCObject* sender) {
    this->updateValues(1);
}

void CheckboxInputControl::onToggler2Toggled(CCObject* sender) {
    this->updateValues(2);
}

CheckboxInputControl* CheckboxInputControl::create(const std::string& labelText,
                                                   const std::shared_ptr<SearchSetting<bool>> relatedSetting1,
                                                   const std::string& labelText2,
                                                   const std::shared_ptr<SearchSetting<bool>> relatedSetting2,
                                                   bool mutuallyExclusive) {
    if (const auto newNode = new CheckboxInputControl(); newNode != nullptr && newNode->init(
        labelText, relatedSetting1, labelText2, relatedSetting2, mutuallyExclusive)) {
        newNode->autorelease();
        return newNode;
    }
    else {
        delete newNode;
        return nullptr;
    }
}

void CheckboxInputControl::saveSetting() {
    relatedSetting1->setSettingValue(settingToggle1->isToggled());
    if (settingToggle2 != nullptr) {
        relatedSetting2->setSettingValue(settingToggle2->isToggled());
    }
}

void CheckboxInputControl::loadSetting() {
    this->settingToggle1->toggle(this->relatedSetting1->getSettingValue());
    if (this->relatedSetting2 != nullptr) {
        this->settingToggle2->toggle(this->relatedSetting2->getSettingValue());
    }
}
