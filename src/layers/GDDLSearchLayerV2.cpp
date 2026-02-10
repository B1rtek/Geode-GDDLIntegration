#include "GDDLSearchLayerV2.h"

#include <Utils.h>
#include <Geode/utils/cocos.hpp>

bool GDDLSearchLayerV2::init() {
    if (!FLAlertLayer::init(150))
        return false;

    createBaseUI();

    return true;
}

void GDDLSearchLayerV2::createBaseUI() {
    // window size and buttons boilerplate
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    // background
    const auto bg = CCScale9Sprite::create(Utils::getGrayPopupBG().c_str(), {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({winSize.width / 2, winSize.height / 2});
    bg->setID("gddl-demon-search-popup"_spr);
    m_mainLayer->addChild(bg, -1);
    const float aspectRatioFixX = (569.0f - winSize.width) / 2;
    const float aspectRatioFixY = (320.0f - winSize.height) / 2;
    // menu with the main layout
    m_buttonMenu = CCMenu::create();
    m_buttonMenu->setContentSize({winSize.width, winSize.height});
    m_buttonMenu->setPosition({64.5f - aspectRatioFixX, 295.0f - aspectRatioFixY});
    m_buttonMenu->setID("gddl-demon-search-menu"_spr);
    m_mainLayer->addChild(m_buttonMenu, 0);
    // close button
    const auto closeButtonSprite = Utils::getGrayPopupCloseButton();
    const auto closeButton =
            CCMenuItemSpriteExtra::create(closeButtonSprite, this, menu_selector(GDDLSearchLayerV2::onClose));
    m_buttonMenu->addChild(closeButton, 1);
    closeButton->setPosition({10.0f, 0.0f});
}

void GDDLSearchLayerV2::onClose(CCObject* sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

GDDLSearchLayerV2* GDDLSearchLayerV2::create() {
    const auto newLayer = new GDDLSearchLayerV2();
    // ReSharper disable once CppDFAConstantConditions
    if (newLayer != nullptr && newLayer->init()) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLSearchLayerV2::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}
