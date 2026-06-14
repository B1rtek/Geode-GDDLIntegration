#include "GDDLThemeListLayer.h"

bool GDDLThemeListLayer::init(bool thickFrame) {
    if (!GDDLThemeBaseLayer::init()) return false;

    // add the frame and list
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    packsList = ScrollLayer::create({356.0f, 220.0f});
    packsList->setPosition({winSize.width / 2 - listSize.x / 2, winSize.height / 2 - listSize.y / 2});
    // for (int i = 0; i < 5; i++) {
    //     // TODO a bunch of placeholders, replace with actual content later
    //     packsList->m_contentLayer->addChild(PackListItem::create(356.0f, PackInfo({}, "Pack " + std::to_string(i+1), "tier_unrated.png")));
    // }
    packsList->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout());
    packsList->scrollToTop();
    this->addChild(packsList);

    if (thickFrame) {
        // not needed for now lol
        createThickFrame();
    } else {
        createThinFrame();
    }

    return true;
}

void GDDLThemeListLayer::createThinFrame() {
    const std::vector<CCPoint> sidePositions = {
        {packsList->getPositionX() - 8.0f, packsList->getContentHeight() / 2 + packsList->getPositionY()}, // left
        {packsList->getContentWidth() / 2  + packsList->getPositionX(), packsList->getContentHeight() + packsList->getPositionY() + 7.5f}, // top
        {packsList->getContentWidth() + packsList->getPositionX() + 8.0f, packsList->getContentHeight() / 2 + packsList->getPositionY()}, // right
        {packsList->getContentWidth() / 2  + packsList->getPositionX(), packsList->getPositionY() - 7.5f} // bottom
    };
    for (int i = 0; i < sidePositions.size(); i++) {
        const auto sideSprite = CCSprite::create(Mod::get()->expandSpriteName("border_thin.png").data());
        sideSprite->setPosition(sidePositions[i]);
        sideSprite->setRotation((i - 1) * 90.0f);
        sideSprite->setScaleX(((i % 2 == 0 ? packsList->getContentHeight() : packsList->getContentWidth())) / sideSprite->getContentWidth());
        this->addChild(sideSprite);
    }
    const std::vector<CCPoint> cornerPositions = {
        {packsList->getPositionX() - 8.0f, packsList->getContentHeight() + packsList->getPositionY() + 7.5f}, // top left
        {packsList->getContentWidth() + packsList->getPositionX() + 8.0f, packsList->getContentHeight() + packsList->getPositionY() + 7.5f}, // top right
        {packsList->getContentWidth() + packsList->getPositionX() + 8.0f, packsList->getPositionY() - 7.5f}, // bottom right
        {packsList->getPositionX() - 8.0f, packsList->getPositionY() - 7.5f} // bottom left
    };
    for (int i = 0; i < cornerPositions.size(); i++) {
        const auto cornerSprite = CCSprite::create(Mod::get()->expandSpriteName("corner_thin.png").data());
        cornerSprite->setPosition(cornerPositions[i]);
        cornerSprite->setRotation(i * 90.0f);
        this->addChild(cornerSprite);
    }
}

void GDDLThemeListLayer::createThickFrame() {

}

GDDLThemeListLayer* GDDLThemeListLayer::create(const bool thickFrame) {
    const auto ret = new GDDLThemeListLayer();
    if (ret->init(thickFrame)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

GDDLThemeListLayer* GDDLThemeListLayer::scene(const bool thickFrame) {
    const auto scene = CCScene::create();
    const auto layer = create(thickFrame);
    scene->addChild(layer);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
    return layer;
}
