#include "TextInputControl.h"

#include <Utils.h>

bool TextInputControl::init(const std::string& labelText, const std::shared_ptr<SearchSetting<std::string>>& relatedSetting) {
    if (!SearchInputControl::init()) return false;

    // the actual UI of the control
    Utils::createLabel(controlMenu, "bigFont.fnt", labelText, standardComponentWidth, {controlSize.x/2, 45.0f});
    Utils::createGeodeTextInput(controlMenu, textInputNode, "bigFont.fnt", "", 32, {standardComponentWidth, standardComponentHeight}, {controlSize.x/2, 15.0f});
    // associate with setting and load the value
    this->relatedSetting = relatedSetting;
    this->loadSetting();
    return true;
}

TextInputControl* TextInputControl::create(const std::string& labelText, const std::shared_ptr<SearchSetting<std::string>>& relatedSetting) {
    if (const auto newNode = new TextInputControl(); newNode != nullptr && newNode->init(labelText, relatedSetting)) {
        newNode->autorelease();
        return newNode;
    } else {
        delete newNode;
        return nullptr;
    }
}

void TextInputControl::saveSetting() {
    this->relatedSetting->setSettingValue(this->textInputNode->getString());
}

void TextInputControl::loadSetting() {
    this->textInputNode->setString(this->relatedSetting->getSettingValue());
}
