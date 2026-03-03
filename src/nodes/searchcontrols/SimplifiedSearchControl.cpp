#include "SimplifiedSearchControl.h"

bool SimplifiedSearchControl::init(SearchObject* searchObject) {
    if (!CheckboxInputControl::init("Completed", searchObject->getCompletedSetting(), "Uncompleted", searchObject->getUncompletedSetting(), true)) return false;

    // resize to fill the whole page, this is a big component
    this->setContentSize(controlSize * 3);
    controlMenu->setContentSize(controlSize * 3);
    this->updateLayout();
    // checkboxes and labels are handled by CheckboxInputControl, we can move them around however
    settingToggle1->setPosition({90.0f, 15.0f});
    settingLabel1->setPosition({150.0f, 15.0f});
    settingToggle2->setPosition({250.0f, 15.0f});
    settingLabel2->setPosition({310.0f, 15.0f});
    // tier buttons
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            int targetTier = row+1+col*rows;
            targetTier = targetTier > Values::highestTier ? -1 : targetTier;
            const auto tierNode = createTierNode(targetTier);
            tierNode->setPosition({tiersGridPosition.x + spacing * col, tiersGridPosition.y - spacing * row});
            controlMenu->addChild(tierNode);
        }
    }
    // associate with SearchObject and load value
    this->searchObject = searchObject;

    return true;
}

CCMenuItemSpriteExtra* SimplifiedSearchControl::createTierNode(const int tier) {
    const std::string tierString = tier != -1 ? std::to_string(tier) : "unrated";
    const std::string tierSpriteName = "tier_" + tierString + ".png";
    const auto tierSprite = CCSprite::create(Mod::get()->expandSpriteName(tierSpriteName.c_str()).data());
    tierSprite->setScale(0.24f);
    tierSprite->setContentSize({30.0f, 30.0f});
    const auto tierButton = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(SimplifiedSearchControl::onTierSearch));
    tierButton->setContentSize({30.0f, 30.0f});
    tierButton->setID("button-tier-" + std::to_string(tier));
    return tierButton;
}

void SimplifiedSearchControl::onTierSearch(CCObject* sender) {
    // get the tier number
    auto *senderNode = typeinfo_cast<CCNode *>(sender);
    const std::string tierStr = senderNode->getID();
    const int start = tierStr.find("button-tier-") + 12;
    const int end = tierStr.size();
    const std::string tierNumberStr = tierStr.substr(start, end - start);
    const Result<int> maybeTierNumber = numFromString<int>(tierNumberStr);
    if (!maybeTierNumber.isOk()) {
        // TODO change this into Notification
        FLAlertLayer::create("Error", "Invalid tier number", "OK")->show();
        return;
    }
    // search
    log::info("Tier clicked: {}", maybeTierNumber.unwrap());
}

SimplifiedSearchControl* SimplifiedSearchControl::create(SearchObject* searchObject) {
    if (const auto newNode = new SimplifiedSearchControl(); newNode != nullptr && newNode->init(searchObject)) {
        newNode->autorelease();
        return newNode;
    } else {
        delete newNode;
        return nullptr;
    }
}
