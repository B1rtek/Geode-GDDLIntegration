#include "GDDLLevelInfoPopup.h"

#include <Geode/utils/web.hpp>

#include "RatingsManager.h"
#include "Utils.h"
#include "nodes/BarChartNode.h"

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
    m_buttonMenu->setContentSize(popupSize);
    m_buttonMenu->setID("button-menu"_spr);
    // m_buttonMenu->setLayout(ColumnLayout::create()->setGap(5.0f)->setAxisReverse(true)); // this won't do
    m_mainLayer->addChild(m_buttonMenu, 10);
    m_buttonMenu->setPosition({ winSize.width / 2, winSize.height / 2 });

    // title
    const auto title = CCLabelBMFont::create("GDDL Level Info", "goldFont.fnt");
    title->setID("gddl-level-info-title"_spr);
    title->setScale(0.9f);
    m_buttonMenu->addChild(title, 1);
    title->setPosition({0.0f, popupSize.y/2-27.5f}); // or basically the standard position below the top border for a title

    // content
    // addLevelInfo();

    // open in browser button
    const auto browserButtonSprite = CCSprite::createWithSpriteFrameName("GJ_redoBtn_001.png");
    browserButtonSprite->setScale(0.9f);
    const auto openInBrowserButton = CCMenuItemSpriteExtra::create(browserButtonSprite, this, menu_selector(GDDLLevelInfoPopup::onOpenInBrowser));
    openInBrowserButton->setID("gddl-level-info-browser"_spr);
    m_buttonMenu->addChild(openInBrowserButton, 9);
    openInBrowserButton->setPosition({-100.0f, -(popupSize.y)/2.0f});

    // ok button
    const auto spr = ButtonSprite::create("OK");
    const auto okButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GDDLLevelInfoPopup::onClose));
    okButton->setID("gddl-level-info-ok"_spr);
    m_buttonMenu->addChild(okButton, 10);
    okButton->setPosition({-130.0f, -(popupSize.y)/2.0f}); // or basically the standard position above the bottom border for a button

    prepareSpreadListener();

    if (RatingsManager::hasSpread(levelID)) {
        addBarCharts();
    } else {
        const auto reqInProgress = CCLabelBMFont::create("Downloading spread...", "chatFont.fnt");
        reqInProgress->setPosition({-100.0f, -40.0f});
        reqInProgress->setID("gddl-level-info-spread-label"_spr);
        m_buttonMenu->addChild(reqInProgress);
        auto req = web::WebRequest();
        spreadListener.setFilter(req.get(getSpreadEndpointUrl(levelID)));
    }

    m_buttonMenu->setPosition({m_buttonMenu->getPositionX(), m_buttonMenu->getPositionY() + 100.0f});

    return true;
}

void GDDLLevelInfoPopup::onClose(CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLLevelInfoPopup::onOpenInBrowser(CCObject *sender) {
    std::string url = "https://gdladder.com/level/" + std::to_string(levelID);
    web::openLinkInBrowser(url);
}

void GDDLLevelInfoPopup::prepareSpreadListener() {
    spreadListener.bind([this] (web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            const RatingsSpread spread = RatingsSpread(jsonResponse);
            RatingsManager::cacheSpread(this->levelID, spread);
            addBarCharts();
        } else if (e->isCancelled()) {
            // :(
        }
    });
}

void GDDLLevelInfoPopup::addBarCharts() {
    RatingsSpread spread = RatingsManager::getSpread(levelID);
    const auto diffSpreadData = spread.getDiffSpreadData();
    const auto diffChart = BarChartNode::create(diffSpreadData, {150.0f, 20.0f * diffSpreadData.size()}, 50.0f, 20.0f);
    diffChart->setPosition({-230.0f, -150.0f});
    const auto enjSpreadData = spread.getEnjSpreadData();
    const auto enjChart = BarChartNode::create(enjSpreadData, {150.0f, 20.0f * enjSpreadData.size()}, 50.0f, 20.0f);
    enjChart->setPosition({0.0f, -200.0f});
    m_buttonMenu->removeChildByID("gddl-level-info-spread-label"_spr);
    m_buttonMenu->addChild(diffChart);
    m_buttonMenu->addChild(enjChart);
}

std::string GDDLLevelInfoPopup::getSpreadEndpointUrl(const int levelID) {
    return "https://gdladder.com/api/level/" + std::to_string(levelID) + "/submissions/spread";
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
    levelInfoLabels->setPosition({0.0f, 5.0f});
}
