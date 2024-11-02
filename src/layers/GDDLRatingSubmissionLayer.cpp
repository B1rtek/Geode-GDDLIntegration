#include "GDDLRatingSubmissionLayer.h"

bool GDDLRatingSubmissionLayer::init(int levelID, int attempts, bool twoPlayer) {
    if(!FLAlertLayer::init(75)) return false; // that magic number is actually bg opacity btw

    const CCPoint popupSize = {280.0f, 185.0f};
    const auto winSize = CCDirector::sharedDirector()->getWinSize();

    // background
    const auto bg = CCScale9Sprite::create("GJ_square05.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });
    bg->setID("gddl-rating-submit-bg"_spr);
    m_mainLayer->addChild(bg, -1);

    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("gddl-rating-submit-menu"_spr);
    m_buttonMenu->setContentSize(popupSize);
    m_buttonMenu->setPosition({ winSize.width / 2 - popupSize.x / 2, winSize.height / 2 - popupSize.y / 2});
    m_mainLayer->addChild(m_buttonMenu, 10);
    // title
    const auto title = CCLabelBMFont::create("Submit rating", "goldFont.fnt");
    title->setID("gddl-rating-submit-title"_spr);
    title->setPosition({popupSize.x / 2, popupSize.y - 20.0f});
    m_buttonMenu->addChild(title);
    // close button
    const auto closeButtonSprite = CircleButtonSprite::createWithSpriteFrameName("geode.loader/close.png", .85f,CircleBaseColor::Gray);
    m_closeBtn = CCMenuItemSpriteExtra::create(closeButtonSprite, this, menu_selector(GDDLRatingSubmissionLayer::onClose));
    m_buttonMenu->addChild(m_closeBtn);
    m_closeBtn->setPosition({3.0f, popupSize.y - 3.0f});

    return true;
}

void GDDLRatingSubmissionLayer::onClose(CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

GDDLRatingSubmissionLayer *GDDLRatingSubmissionLayer::create(int levelID, int attempts, bool twoPlayer) {
    if (const auto newLayer = new GDDLRatingSubmissionLayer(); newLayer != nullptr && newLayer->init(levelID, attempts, twoPlayer)) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLRatingSubmissionLayer::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}
