#include "GDDLPacksLayer.h"

#include <Utils.h>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <managers/PacksManager.h>
#include <nodes/PackListItem.h>
#include <nodes/searchcontrols/CheckboxInputControl.h>
#include <objects/searchsettings/BoolSearchSetting.h>

bool GDDLPacksLayer::init() {
    if (!GDDLThemeListLayer::init(false)) return false;
    log::info("[({})GDDLPacksLayer::init] Called", fmt::ptr(this));

    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    // base UI already created by GDDLThemeListLayer

    // next/prev page arrows, positions are really weird because math doesn't work for some reason here
    const auto nextPageMenu = CCMenu::create();
    nextPageMenu->setContentSize({50.0f, 50.0f});
    nextPageMenu->setPosition({120.0f + listSize.x, winSize.height / 2 - 25.0f});
    this->addChild(nextPageMenu);
    const auto nextButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextButtonSprite->setFlipX(true);
    const auto nextPageButton = CCMenuItemSpriteExtra::create(nextButtonSprite, this, menu_selector(GDDLPacksLayer::onNextPage));
    nextPageButton->setPosition({25.0f, 25.0f});
    nextPageMenu->addChild(nextPageButton);

    const auto prevPageMenu = CCMenu::create();
    prevPageMenu->setContentSize({50.0f, 50.0f});
    prevPageMenu->setPosition({44.0f, winSize.height / 2 - 25.0f});
    this->addChild(prevPageMenu);
    const auto prevButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    const auto prevPageButton = CCMenuItemSpriteExtra::create(prevButtonSprite, this, menu_selector(GDDLPacksLayer::onPrevPage));
    prevPageButton->setPosition({25.0f, 25.0f});
    prevPageMenu->addChild(prevPageButton);

    // refresh button
    const auto refreshButtonMenu = CCMenu::create();
    refreshButtonMenu->setContentSize({50.0f, 50.0f});
    refreshButtonMenu->setPosition({winSize.width - 50.0f, 25.0f});
    this->addChild(refreshButtonMenu);
    const auto refreshButtonSprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    const auto refreshButton = CCMenuItemSpriteExtra::create(refreshButtonSprite, this, menu_selector(GDDLPacksLayer::onRefresh));
    refreshButton->setPosition({20.0f, 5.0f});
    refreshButtonMenu->addChild(refreshButton);

    // title label
    titleLabel = CCLabelBMFont::create("Packs", "bigFont.fnt");
    titleLabel->setPosition({winSize.width / 2, winSize.height / 2 + listSize.y / 2 + 30.0f});
    this->addChild(titleLabel);

    log::info("[({})GDDLPacksLayer::init] Created, getting packs", fmt::ptr(this));

    // get packs
    PacksManager::subscribeToObservers(this);
    log::info("[({})GDDLPacksLayer::init] Subscribed to PacksManager", fmt::ptr(this));
    const auto maybePackCategoryInfo = PacksManager::getOrRequestPackCategoryInfo(page);
    if (maybePackCategoryInfo.isOk()) {
        log::info("[({})GDDLPacksLayer::init] Received PackCategoryInfo from cache for category {}", fmt::ptr(this), maybePackCategoryInfo.unwrap().getId());
        PacksManager::unsubscribeFromObservers(this);
        log::info("[({})GDDLPacksLayer::init] Unsubscribed from PacksManager", fmt::ptr(this));
        updateDisplay();
        // from now on all data is loaded
    }

    return true;
}

void GDDLPacksLayer::updateDisplay() {
    log::info("[({})GDDLPacksLayer::updateDisplay] Called", fmt::ptr(this));
    scrollList->m_contentLayer->removeAllChildren();
    const std::vector<std::shared_ptr<PackInfo>> packInfos = PacksManager::getPacksFromCategory(page);
    log::info("[({})GDDLPacksLayer::updateDisplay] Adding PackListItems", fmt::ptr(this));
    bool dark = true;
    for (const auto& packInfo : packInfos) {
        scrollList->m_contentLayer->addChild(PackListItem::create(356.0f, packInfo->getId(), dark));
        dark = !dark;
    }
    scrollList->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout(0));
    scrollList->scrollToTop();
    log::info("[({})GDDLPacksLayer::updateDisplay] Updating title", fmt::ptr(this));
    titleLabel->setString(PacksManager::getOrRequestPackCategoryInfo(page).unwrap().getName().c_str());
}

void GDDLPacksLayer::onNextPage(CCObject* sender) {
    log::info("[({})GDDLPacksLayer::onNextPage] Called", fmt::ptr(this));
    ++page;
    if (page > PacksManager::getCategoryCount()) page = 1;
    updateDisplay();
}

void GDDLPacksLayer::onPrevPage(CCObject* sender) {
    log::info("[({})GDDLPacksLayer::onPrevPage] Called", fmt::ptr(this));
    --page;
    if (page < 1) page = PacksManager::getCategoryCount();
    updateDisplay();
}

void GDDLPacksLayer::onBack(CCObject* sender) {
    log::info("[({})GDDLPacksLayer::onBack] Called", fmt::ptr(this));
    backActions();
}

void GDDLPacksLayer::onRefresh(CCObject* sender) {
    log::info("[({})GDDLPacksLayer::onRefresh] Called", fmt::ptr(this));
    // TODO some kind of a loading circle
    PacksManager::subscribeToObservers(this);
    log::info("[({})GDDLPacksLayer::onRefresh] Subscribed to PacksManager", fmt::ptr(this));
    PacksManager::requestPackListRefresh();
    log::info("[({})GDDLPacksLayer::onRefresh] Requested pack list refresh", fmt::ptr(this));
}

void GDDLPacksLayer::keyBackClicked() {
    backActions();
}

void GDDLPacksLayer::backActions() {
    log::info("[({})GDDLPacksLayer::backActions] Called", fmt::ptr(this));
    PacksManager::unsubscribeFromObservers(this);
    log::info("[({})GDDLPacksLayer::backActions] Unsubscribed from PacksManager", fmt::ptr(this));
    CCDirector::get()->popScene();
}

GDDLPacksLayer* GDDLPacksLayer::create() {
    const auto ret = new GDDLPacksLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

GDDLPacksLayer* GDDLPacksLayer::scene() {
    const auto scene = CCScene::create();
    const auto layer = create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
    return layer;
}

void GDDLPacksLayer::updateData() {
    log::info("[({})GDDLPacksLayer::updateData] Called", fmt::ptr(this));
    PacksManager::unsubscribeFromObservers(this);
    log::info("[({})GDDLPacksLayer::updateData] Unsubscribed from PacksManager", fmt::ptr(this));
    updateDisplay();
}
