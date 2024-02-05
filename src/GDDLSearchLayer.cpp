#include "GDDLSearchLayer.h"
bool GDDLSearchLayer::init() {
    if(!FLAlertLayer::init(75)) return false;

    CCPoint popupSize = {360.0f, 200.0f};
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    // background
    auto bg = CCScale9Sprite::create("GJ_square01.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });
    bg->setID("gddl-demon-search-popup"_spr);
    m_mainLayer->addChild(bg, -1);
    // menu with the main layout
    m_buttonMenu = CCMenu::create();
    m_buttonMenu->setID("gddl-demon-search-menu"_spr);
    m_buttonMenu->setLayout(ColumnLayout::create()->setGap(5.0f)->setAxisReverse(true)->setAutoScale(true));
    m_mainLayer->addChild(m_buttonMenu, 10);
    // title
    auto title = CCLabelBMFont::create("GDDL Search", "goldFont.fnt");
    title->setID("gddl-demon-search-title"_spr);
    m_buttonMenu->addChild(title);
    m_buttonMenu->reorderChild(title, 0);
    // tier buttons
    for (int row = 0; row < 5; row++) {
        auto rowNode = CCMenu::create();
        rowNode->setLayout(RowLayout::create()->setGap(5.0f));
        rowNode->setContentSize({popupSize.x-10.0f, 20.0f});
        for (int column = 0; column < 7; column++) {
            auto tierNode = createTierNode(row+1+column*5);
            rowNode->addChild(tierNode);
        }
        rowNode->updateLayout();
        m_buttonMenu->addChild(rowNode);
        m_buttonMenu->reorderChild(rowNode, row+1);
    }
    // ok button
    auto spr = ButtonSprite::create("OK");
    auto okButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GDDLSearchLayer::onClose));
    okButton->setID("gddl-demon-split-ok"_spr);
    m_buttonMenu->addChild(okButton);
    m_buttonMenu->reorderChild(okButton, 10);
    m_buttonMenu->updateLayout();

    return true;
}

void GDDLSearchLayer::onClose(CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLSearchLayer::onSearchOptionSelected(CCObject *sender) {

}

void GDDLSearchLayer::onTierSearch(CCObject *sender) {
    FLAlertLayer::create("GDDL Search", "Search clicked", "OK")->show();
}

CCMenuItemSpriteExtra *GDDLSearchLayer::createTierNode(int tier) {
    // tier sprite
    std::string tierSpriteName = "tier_" + std::to_string(tier) + ".png";
    auto textureName = Mod::get()->expandSpriteName(tierSpriteName.c_str());
    auto tierSprite = CCSprite::create(textureName);
    tierSprite->setScale(0.15f);
    tierSprite->setContentSize({20.0f, 20.0f});
    auto tierButton = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(GDDLSearchLayer::onTierSearch));
    tierButton->setContentSize({20.0f, 20.0f});
    tierButton->setID("button-tier-"+std::to_string(tier));
    return tierButton;
}

GDDLSearchLayer *GDDLSearchLayer::create() {
    const auto newLayer = new GDDLSearchLayer();
    if(newLayer != nullptr && newLayer->init()) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLSearchLayer::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}
