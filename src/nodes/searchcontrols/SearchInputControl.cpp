#include "SearchInputControl.h"

bool SearchInputControl::init() {
    if (!CCNode::init()) return false;

    this->setContentSize(controlSize);
    controlMenu = new CCMenu();
    controlMenu->setContentSize(controlSize);
    controlMenu->setPosition({0.0f, 0.0f});
    controlMenu->setZOrder(2);
    this->addChild(controlMenu);
    // handleTouchPriority(this);

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

void SearchInputControl::relocateToGlobalMenu(CCMenu* popupMainMenu) {
    CCPoint positionCorrection = {0.0f, 0.0f};
    // FUCK TOUCH PRIO
    // because nested menus do not work, we have to
    // 1. move the elements to the provided menu
    // 2. map their position in the other menu
    const auto mainMenuParent = popupMainMenu->getParent();
    auto currentParent = controlMenu->getParent();
    while (currentParent != mainMenuParent) {
        positionCorrection += currentParent->getPosition();
        currentParent = currentParent->getParent();
    }
    positionCorrection -= popupMainMenu->getPosition();
    // move all children
    std::vector<CCNode*> children;
    for (const auto child : CCArrayExt<CCNode*>(controlMenu->getChildren())) {
        children.push_back(child);
    }
    for (const auto child : children) {
        controlMenu->removeChild(child, false);
        popupMainMenu->addChild(child);
        child->setPosition({child->getPositionX() + positionCorrection.x, child->getPositionY() + positionCorrection.y});
    }
}
