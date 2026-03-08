#include "GDDLPacksLayer.h"

#include <Geode/ui/Layout.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <nodes/searchcontrols/CheckboxInputControl.h>
#include <objects/searchsettings/BoolSearchSetting.h>

bool GDDLPacksLayer::init() {
    if (!CCLayer::init()) return false;

    // bg
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    const auto bg = CCSprite::create("GJ_gradientBG.png");
    bg->setAnchorPoint({0.0f, 0.0f});
    bg->setPosition({-5.0f, -5.0f});
    bg->setZOrder(-3);
    bg->setScaleX((winSize.width + 10.0f) / bg->getContentWidth());
    bg->setScaleY((winSize.height + 10.0f) / bg->getContentHeight());
    this->addChild(bg);

    // TODO cornerpieces

    // definitely not taken from Geode's ModsLayer
    const auto backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    backMenu->setContentSize({100.f, 40.f});
    backMenu->setAnchorPoint({ .0f, .5f });

    const auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    const auto backBtn = CCMenuItemSpriteExtra::create(
        backSpr, this, menu_selector(GDDLPacksLayer::onBack)
    );
    backBtn->setID("back-button");
    backMenu->addChild(backBtn);
    backMenu->setLayout(SimpleRowLayout::create()->setMainAxisAlignment(MainAxisAlignment::Start)->setGap(5.f));
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(8, -23), false);

    // list
    packsList = ScrollLayer::create({356.0f, 220.0f});
    packsList->setPosition({winSize.width / 2 - listSize.x / 2, winSize.height / 2 - listSize.y / 2});
    for (int i = 0; i < 5; i++) {
        // TODO a bunch of placeholders, replace with actual content later
        packsList->m_contentLayer->addChild(CheckboxInputControl::create("Test item " + std::to_string(i), std::make_shared<BoolSearchSetting>("test-setting-bool", false)));
    }
    packsList->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout());
    packsList->scrollToTop();
    packsList->m_contentLayer->setColor(ccc3(255, 0, 0)); // super evil list
    packsList->m_contentLayer->setOpacity(100);
    this->addChild(packsList);
    createListFrame();

    return true;
}

void GDDLPacksLayer::createListFrame() {
    const std::vector<CCPoint> sidePositions = {
        {packsList->getPositionX() - 8.0f, packsList->getContentHeight() / 2 + packsList->getPositionY()}, // left
        {packsList->getContentWidth() / 2  + packsList->getPositionX(), packsList->getContentHeight() + packsList->getPositionY() + 7.5f}, // top
        {packsList->getContentWidth() + packsList->getPositionX() + 8.0f, packsList->getContentHeight() / 2 + packsList->getPositionY()}, // right
        {packsList->getContentWidth() / 2  + packsList->getPositionX(), packsList->getPositionY() - 7.5f} // bottom
    };
    for (int i = 0; i < sidePositions.size(); i++) {
        const auto sideSprite = CCSprite::createWithSpriteFrameName("GJ_table_side_001.png");
        sideSprite->setPosition(sidePositions[i]);
        sideSprite->setRotation(i * 90.0f);
        sideSprite->setScaleY(((i % 2 == 0 ? packsList->getContentHeight() : packsList->getContentWidth()) + 2.0f) / sideSprite->getContentHeight());
        this->addChild(sideSprite);
    }
    const std::vector<CCPoint> cornerPositions = {
        {packsList->getPositionX() - 8.0f, packsList->getContentHeight() + packsList->getPositionY() + 7.5f}, // top left
        {packsList->getContentWidth() + packsList->getPositionX() + 8.0f, packsList->getContentHeight() + packsList->getPositionY() + 7.5f}, // top right
        {packsList->getContentWidth() + packsList->getPositionX() + 8.0f, packsList->getPositionY() - 7.5f}, // bottom right
        {packsList->getPositionX() - 8.0f, packsList->getPositionY() - 7.5f} // bottom left
    };
    for (int i = 0; i < cornerPositions.size(); i++) {
        const auto cornerSprite = CCSprite::createWithSpriteFrameName("GJ_table_corner_001.png");
        cornerSprite->setPosition(cornerPositions[i]);
        cornerSprite->setRotation(i * 90.0f);
        this->addChild(cornerSprite);
    }
}

void GDDLPacksLayer::onBack(CCObject* sender) {
    backActions();
}

void GDDLPacksLayer::keyBackClicked() {
    backActions();
}

void GDDLPacksLayer::backActions() {
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
