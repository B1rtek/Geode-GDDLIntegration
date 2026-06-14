#include "GDDLThemeBaseLayer.h"

bool GDDLThemeBaseLayer::init() {
    if (!CCLayer::init()) return false;

    // bg
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    const auto bg = CCSprite::create(Mod::get()->expandSpriteName("bg.png").data());
    bg->setAnchorPoint({0.0f, 0.0f});
    bg->setPosition({-5.0f, -5.0f});
    bg->setZOrder(-3);
    bg->setScaleX((winSize.width + 10.0f) / bg->getContentWidth());
    bg->setScaleY((winSize.height + 10.0f) / bg->getContentHeight());
    this->addChild(bg);

    // cornerpieces - texture name, position, anchor point
    const std::vector<std::tuple<std::string, CCPoint, CCPoint>> cornerpiecePlacements = {
        {"spike_ll.png", {0.0f, 0.0f}, {0.0f, 0.0f}},
        {"spike_lr.png", {winSize.width, 0.0f}, {1.0f, 0.0f}},
        {"spike_ur.png", {winSize.width, winSize.height}, {1.0f, 1.0f}},
        {"spike_ul.png", {0.0f, winSize.height}, {0.0f, 1.0f}}
    };
    for (const auto placement: cornerpiecePlacements) {
        const auto spikeTexture = CCSprite::create(Mod::get()->expandSpriteName(std::get<0>(placement)).data());
        spikeTexture->setAnchorPoint(std::get<2>(placement));
        spikeTexture->setPosition(std::get<1>(placement));
        this->addChild(spikeTexture);
    }

    // definitely not taken from Geode's ModsLayer
    const auto backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    backMenu->setContentSize({100.f, 40.f});
    backMenu->setAnchorPoint({ .0f, .5f });

    const auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    const auto backBtn = CCMenuItemSpriteExtra::create(
        backSpr, this, menu_selector(GDDLThemeBaseLayer::onBack)
    );
    backBtn->setID("back-button");
    backMenu->addChild(backBtn);
    backMenu->setLayout(SimpleRowLayout::create()->setMainAxisAlignment(MainAxisAlignment::Start)->setGap(5.f));
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(8, -23), false);

    return true;
}

void GDDLThemeBaseLayer::onBack(CCObject* sender) {
    backActions();
}

void GDDLThemeBaseLayer::keyBackClicked() {
    backActions();
}

void GDDLThemeBaseLayer::backActions() {
    CCDirector::get()->popScene();
}

GDDLThemeBaseLayer* GDDLThemeBaseLayer::create() {
    const auto ret = new GDDLThemeBaseLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

GDDLThemeBaseLayer* GDDLThemeBaseLayer::scene() {
    const auto scene = CCScene::create();
    const auto layer = create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
    return layer;
}
