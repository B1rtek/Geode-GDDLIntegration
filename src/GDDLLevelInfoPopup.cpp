#include "GDDLLevelInfoPopup.h"
#include "RatingsManager.h"
#include "Utils.h"

bool GDDLLevelInfoPopup::init() {
    if(!FLAlertLayer::init(150)) return false;

    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    const auto bg = CCScale9Sprite::create("square01_001.png", {0.0f, 0.0f, 94.0f, 94.0f});

    bg->setContentSize(popupSize);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });
    bg->setID("gddl-level-info-popup"_spr);
    m_mainLayer->addChild(bg, -1);

    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("button-menu"_spr);
    m_buttonMenu->setLayout(ColumnLayout::create()->setGap(5.0f)->setAxisReverse(true)->setAutoScale(true));
    m_mainLayer->addChild(m_buttonMenu, 10);

    // title
    const auto title = CCLabelBMFont::create("GDDL Level Info", "goldFont.fnt");
    title->setID("gddl-level-info-title"_spr);
    title->setScale(0.9f);
    m_buttonMenu->addChild(title, 1);

    // content
    addLevelInfo();

    // open in browser button

    // ok button
    const auto spr = ButtonSprite::create("OK");
    const auto okButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GDDLLevelInfoPopup::onClose));
    okButton->setID("gddl-level-info-ok"_spr);
    m_buttonMenu->addChild(okButton);
    m_buttonMenu->reorderChild(okButton, 10);

    m_buttonMenu->updateLayout();
    m_mainLayer->updateLayout();
    return true;
}

void GDDLLevelInfoPopup::onClose(CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

GDDLLevelInfoPopup *GDDLLevelInfoPopup::create(int levelID) {
    // ReSharper disable once CppDFAConstantConditions
    if (const auto newLayer = new GDDLLevelInfoPopup(levelID); newLayer != nullptr && newLayer->init()) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLLevelInfoPopup::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}

GDDLLevelInfoPopup::GDDLLevelInfoPopup(int levelId) : levelID(levelId) {}

void GDDLLevelInfoPopup::addLevelInfo() {
    const auto gddlRating = RatingsManager::getRating(levelID);
    std::string levelInfoText = "Still loading...";
    if(gddlRating) {
        const auto info = gddlRating.value();
        const std::string rating = "<cr>Tier:</c> " + (info.rating == -1 ? "N/A" : Utils::floatToString(info.rating, 2)) + '\n';
        const std::string enjoyment = "<cg>Enjoyment:</c> " + (info.enjoyment == -1 ? "N/A" : Utils::floatToString(info.enjoyment, 2)) + '\n';
        const std::string submissionCount = "<cy>Total submissions:</c> " + (info.submissionCount == -1 ? "N/A" : std::to_string(info.submissionCount)) + '\n';
        levelInfoText = " \n" + rating + enjoyment + submissionCount + " \n"; // uhhh this is a good workaround right?
    }
    const auto levelInfoLabels = TextArea::create(levelInfoText, "chatFont.fnt", 1, popupSize.x-30.0f, {0.5f,0.5f}, 20, false);
    m_buttonMenu->addChild(levelInfoLabels, 2);
}
