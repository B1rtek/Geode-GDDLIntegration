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

    // title label
    titleLabel = CCLabelBMFont::create("Packs", "bigFont.fnt");
    titleLabel->setPosition({winSize.width / 2, winSize.height / 2 + listSize.y / 2 + 30.0f});
    this->addChild(titleLabel);

    // get packs
    PacksManager::subscribeToObservers(this);
    const auto maybePackCategoryInfo = PacksManager::getOrRequestPackCategoryInfo(page);
    if (maybePackCategoryInfo.isOk()) {
        updateDisplay();
        PacksManager::unsubscribeFromObservers(this);
        // from now on all data is loaded
    }

    return true;
}

void GDDLPacksLayer::updateDisplay() {
    scrollList->m_contentLayer->removeAllChildren();
    const std::vector<std::shared_ptr<PackInfo>> packInfos = PacksManager::getPacksFromCategory(page);
    for (const auto& packInfo : packInfos) {
        scrollList->m_contentLayer->addChild(PackListItem::create(356.0f, packInfo->getId()));
    }
    scrollList->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout());
    scrollList->scrollToTop();
    titleLabel->setString(PacksManager::getOrRequestPackCategoryInfo(page).unwrap().getName().c_str());
}

void GDDLPacksLayer::onNextPage(CCObject* sender) {
    ++page;
    if (page > PacksManager::getCategoryCount()) page = 1;
    updateDisplay();
}

void GDDLPacksLayer::onPrevPage(CCObject* sender) {
    --page;
    if (page < 1) page = PacksManager::getCategoryCount();
    updateDisplay();
}

void GDDLPacksLayer::onBack(CCObject* sender) {
    backActions();
}

void GDDLPacksLayer::keyBackClicked() {
    backActions();
}

void GDDLPacksLayer::backActions() {
    PacksManager::unsubscribeFromObservers(this);
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
    PacksManager::unsubscribeFromObservers(this);
    updateDisplay();
}
