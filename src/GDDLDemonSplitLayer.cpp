#include "GDDLDemonSplitLayer.h"

bool GDDLDemonSplitLayer::init() {
    if(!FLAlertLayer::init(75)) return false; // that magic number is actualy bg opacity btw

    CCPoint popupSize = {360.0f, 180.0f};
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    // background
    auto bg = CCScale9Sprite::create("GJ_square02.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });
    bg->setID("gddl-demon-split-bg"_spr);
    m_mainLayer->addChild(bg, -1);
    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("button-menu"_spr);
    m_buttonMenu->setLayout(ColumnLayout::create()->setGap(5.0f)->setAxisReverse(true)->setAutoScale(true));
    m_mainLayer->addChild(m_buttonMenu, 10);
    // title
    auto title = CCLabelBMFont::create("GDDL Demon Split", "goldFont.fnt");
    title->setID("gddl-demon-split-title"_spr);
    m_buttonMenu->addChild(title);
    m_buttonMenu->reorderChild(title, 0);
    // this is where the tiers go

    // ok button
    auto spr = ButtonSprite::create("OK");
    auto okButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GDDLDemonSplitLayer::onClose));
    okButton->setID("gddl-demon-split-ok"_spr);
    m_buttonMenu->addChild(okButton);
    m_buttonMenu->reorderChild(okButton, 1);

    m_buttonMenu->updateLayout();
    m_mainLayer->updateLayout();
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
