#include "GDDLSearchLayerV2.h"

#include <Utils.h>
#include <Geode/utils/cocos.hpp>
#include <nodes/searchcontrols/TextInputControl.h>

bool GDDLSearchLayerV2::init() {
    if (!FLAlertLayer::init(150))
        return false;

    createBaseUI();
    searchObject.loadSettings();
    displayPage(currentPageNumber);

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
    // page
    currentPage = SearchLayerPage::create();
    currentPage->setPosition({winSize.width / 2 - popupSize.x/2 + 2.5f, winSize.height / 2 - popupSize.y/2 + 40.0f});
    m_mainLayer->addChild(currentPage);
}

void GDDLSearchLayerV2::displayPage(int pageNumber) {
    currentPage->clearContent();
    if (pageNumber == -1) {
        // simplified search page
    } else if (pageNumber == 0) {
        // first page
        currentPage->addControl(TextInputControl::create("Level name", searchObject.getLevelNameSetting()));
    }
    cocos::handleTouchPriority(this);
}

void GDDLSearchLayerV2::onClose(CCObject* sender) {
    currentPage->saveSettings();
    searchObject.saveSettings();
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
