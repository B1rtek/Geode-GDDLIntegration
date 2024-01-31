#include "GDDLDemonSplitLayer.h"

bool GDDLDemonSplitLayer::init() {
    if(!FLAlertLayer::init(0)) return false; // that magic number is actualy bg opacity btw



    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("button-menu"_spr);
    m_mainLayer->addChild(m_buttonMenu, 10);

    auto userName = cocos2d::CCLabelBMFont::create("gddl test", "bigFont.fnt");
    userName->setPosition({0, 0});
    userName->setScale(0.5f);
    userName->setID("title-label"_spr);
    m_buttonMenu->addChild(userName);

    auto buttonSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    buttonSprite->setScale(0.5f);
    auto button = CCMenuItemSpriteExtra::create(
        buttonSprite,
        this,
        menu_selector(GDDLDemonSplitLayer::onClose)
    );
    m_buttonMenu->addChild(button);
    button->setPosition({30.0f, 30.0f});

    return true;
}

void GDDLDemonSplitLayer::onClose(cocos2d::CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

GDDLDemonSplitLayer *GDDLDemonSplitLayer::create() {
    if (const auto newLayer = new GDDLDemonSplitLayer(); newLayer != nullptr && newLayer->init()) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}
