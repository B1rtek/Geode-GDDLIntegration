#include "GDDLPacksLayer.h"

#include <Geode/ui/Layout.hpp>

bool GDDLPacksLayer::init() {
    if (!CCLayer::init()) return false;

    auto backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    backMenu->setContentSize({100.f, 40.f});
    backMenu->setAnchorPoint({ .0f, .5f });

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(
        backSpr, this, menu_selector(GDDLPacksLayer::onBack)
    );
    backBtn->setID("back-button");
    backMenu->addChild(backBtn);
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(8, -23), false);

    return true;
}

void GDDLPacksLayer::onBack(CCObject* sender) {
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
