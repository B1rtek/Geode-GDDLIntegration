#include "EnumInputControl.h"

#include <Utils.h>

bool EnumInputControl::init(const std::string& labelText, const std::shared_ptr<EnumSearchSetting> relatedSetting) {
    if (!SearchInputControl::init()) return false;

    // UI
    Utils::createLabel(controlMenu, "bigFont.fnt", labelText, standardComponentWidth, {controlSize.x/2, 45.0f});
    auto bg = Utils::createLabelForChoice(controlMenu, enumLabel, "bigFont.fnt", "", standardComponentWidth, {controlSize.x/2, 15.0f}, {standardComponentWidth, standardComponentHeight});
    Utils::createLeftRightButtonsAround(bg, {13.0f, 19.0f}, this, menu_selector(EnumInputControl::onEnumValueLeft),
                                     menu_selector(EnumInputControl::onEnumValueRight), 10);
    // association with setting
    this->relatedSetting = relatedSetting;
    this->enumLabel->setString(this->relatedSetting->getEnumValue().c_str());
    Utils::scaleLabelToWidth(this->enumLabel, standardComponentWidth);
    return true;
}

void EnumInputControl::onEnumValueLeft(CCObject* sender) {
    const int newValue = this->relatedSetting->getSettingValue() - 1;
    this->relatedSetting->setSettingValue(newValue);
    this->enumLabel->setString(this->relatedSetting->getEnumValue().c_str());
    Utils::scaleLabelToWidth(this->enumLabel, standardComponentWidth);
}

void EnumInputControl::onEnumValueRight(CCObject* sender) {
    const int newValue = this->relatedSetting->getSettingValue() + 1;
    this->relatedSetting->setSettingValue(newValue);
    this->enumLabel->setString(this->relatedSetting->getEnumValue().c_str());
    Utils::scaleLabelToWidth(this->enumLabel, standardComponentWidth);
}

EnumInputControl* EnumInputControl::create(const std::string& labelText,
                                           const std::shared_ptr<EnumSearchSetting> relatedSetting) {
    if (const auto newNode = new EnumInputControl(); newNode != nullptr && newNode->init(labelText, relatedSetting)) {
        newNode->autorelease();
        return newNode;
    } else {
        delete newNode;
        return nullptr;
    }
}
