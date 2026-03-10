#include "PackListItem.h"

#include <Geode/loader/Mod.hpp>

bool PackListItem::init(const float width, const PackInfo& packInfo) {
    if (!CCNode::init()) return false;

    this->setContentSize({width, itemHeight});

    // icon
    const auto icon = CCSprite::create(Mod::get()->expandSpriteName(packInfo.getPackIconPath()).data());
    icon->setScale(0.275f);
    icon->setPosition({20.0f, itemHeight / 2});
    this->addChild(icon);

    // title
    const auto title = CCLabelBMFont::create(packInfo.getPackName().c_str(), "bigFont.fnt");
    title->setAnchorPoint({0.0f, 0.5f});
    title->setScale(0.6f);
    title->setPosition({50.0f, itemHeight / 2});
    this->addChild(title);

    // TODO add completion status

    // view button
    const auto buttonMenu = CCMenu::create();
    buttonMenu->setContentSize({itemHeight * 2, itemHeight});
    buttonMenu->setPosition({width - itemHeight * 2, 0});
    this->addChild(buttonMenu);
    const auto viewButtonSprite = ButtonSprite::create("View", "bigFont.fnt", "GJ_button_01.png");
    viewButtonSprite->setScale(0.6f);
    const auto viewButton = CCMenuItemSpriteExtra::create(viewButtonSprite, this, menu_selector(PackListItem::onView));
    viewButton->setPosition({itemHeight, itemHeight / 2});
    buttonMenu->addChild(viewButton);

    return true;
}

void PackListItem::onView(CCObject* sender) {
    FLAlertLayer::create("view", "you clicked view", "OK")->show();
}

PackListItem* PackListItem::create(const float width, const PackInfo& packInfo) {
    const auto ret = new PackListItem();
    if (ret->init(width, packInfo)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
