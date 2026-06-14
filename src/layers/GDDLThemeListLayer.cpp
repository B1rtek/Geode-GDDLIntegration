#include "GDDLThemeListLayer.h"

bool GDDLThemeListLayer::init(bool thickFrame) {
    if (!GDDLThemeBaseLayer::init()) return false;

    // add the frame and list
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    scrollList = ScrollLayer::create(listSize);
    scrollList->setPosition({winSize.width / 2 - listSize.x / 2, winSize.height / 2 - listSize.y / 2 - 5.0f});
    scrollList->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout());
    scrollList->scrollToTop();
    this->addChild(scrollList);

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
        {scrollList->getPositionX() - 8.0f, scrollList->getContentHeight() / 2 + scrollList->getPositionY()}, // left
        {scrollList->getContentWidth() / 2  + scrollList->getPositionX(), scrollList->getContentHeight() + scrollList->getPositionY() + 7.5f}, // top
        {scrollList->getContentWidth() + scrollList->getPositionX() + 8.0f, scrollList->getContentHeight() / 2 + scrollList->getPositionY()}, // right
        {scrollList->getContentWidth() / 2  + scrollList->getPositionX(), scrollList->getPositionY() - 7.5f} // bottom
    };
    for (int i = 0; i < sidePositions.size(); i++) {
        const auto sideSprite = CCSprite::create(Mod::get()->expandSpriteName("border_thin.png").data());
        sideSprite->setPosition(sidePositions[i]);
        sideSprite->setRotation((i - 1) * 90.0f);
        sideSprite->setScaleX(((i % 2 == 0 ? scrollList->getContentHeight() : scrollList->getContentWidth())) / sideSprite->getContentWidth());
        this->addChild(sideSprite);
    }
    const std::vector<CCPoint> cornerPositions = {
        {scrollList->getPositionX() - 8.0f, scrollList->getContentHeight() + scrollList->getPositionY() + 7.5f}, // top left
        {scrollList->getContentWidth() + scrollList->getPositionX() + 8.0f, scrollList->getContentHeight() + scrollList->getPositionY() + 7.5f}, // top right
        {scrollList->getContentWidth() + scrollList->getPositionX() + 8.0f, scrollList->getPositionY() - 7.5f}, // bottom right
        {scrollList->getPositionX() - 8.0f, scrollList->getPositionY() - 7.5f} // bottom left
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
