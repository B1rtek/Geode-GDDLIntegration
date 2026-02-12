#include "TextInputControl.h"

#include <Utils.h>

bool TextInputControl::init(const std::string& labelText) {
    if (!SearchInputControl::init()) return false;

    // the actual UI of the control
    Utils::createLabel(controlMenu, "bigFont.fnt", labelText, standardComponentWidth, {controlSize.x/2, 45.0f});
    Utils::createGeodeTextInput(controlMenu, textInputNode, "bigFont.fnt", "", 32, {standardComponentWidth, standardComponentHeight}, {controlSize.x/2, 15.0f});
    return true;
}

TextInputControl* TextInputControl::create(const std::string& labelText) {
    if (const auto newNode = new TextInputControl(); newNode != nullptr && newNode->init(labelText)) {
        newNode->autorelease();
        return newNode;
    } else {
        delete newNode;
        return nullptr;
    }
}
