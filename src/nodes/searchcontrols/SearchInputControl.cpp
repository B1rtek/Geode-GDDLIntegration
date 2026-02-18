#include "SearchInputControl.h"

bool SearchInputControl::init() {
    if (!CCNode::init()) return false;

    this->setContentSize(controlSize);
    controlMenu = new CCMenu();
    controlMenu->setContentSize(controlSize);
    controlMenu->setPosition({0.0f, 0.0f});
    controlMenu->setZOrder(2);
    this->addChild(controlMenu);
    handleTouchPriority(this);

    return true;
}

SearchInputControl* SearchInputControl::create() {
    if (const auto newNode = new SearchInputControl(); newNode != nullptr && newNode->init()) {
        newNode->autorelease();
        return newNode;
    } else {
        delete newNode;
        return nullptr;
    }
}
