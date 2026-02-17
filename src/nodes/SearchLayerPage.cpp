#include "SearchLayerPage.h"

bool SearchLayerPage::init() {
    if (!CCNode::init()) return false;

    this->setContentSize(pageSize);

    return true;
}

SearchLayerPage* SearchLayerPage::create() {
    if (const auto newNode = new SearchLayerPage(); newNode != nullptr && newNode->init()) {
        newNode->autorelease();
        return newNode;
    } else {
        delete newNode;
        return nullptr;
    }
}

void SearchLayerPage::addControl(SearchInputControl* control) {
    if (controls.size() >= 9) return;
    control->setPosition(getControlPosition(controls.size()));
    this->addChild(control);
    controls.push_back(control);
}

void SearchLayerPage::clearContent() {
    for (const auto control : controls) {
        this->removeChild(control);
    }
    controls.clear();
}

void SearchLayerPage::saveSettings() {
    for (const auto control : controls) {
        control->saveSetting();
    }
}

CCPoint SearchLayerPage::getControlPosition(const unsigned index) {
    const float xPos = pageSize.x/3 * static_cast<float>(index/3);
    const float yPos = 2*pageSize.y/3 - pageSize.y/3 * static_cast<float>(index%3);
    return {xPos, yPos};
}
