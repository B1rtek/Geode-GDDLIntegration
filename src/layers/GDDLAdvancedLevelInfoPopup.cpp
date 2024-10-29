#include "GDDLAdvancedLevelInfoPopup.h"
#include "RatingsManager.h"
#include "objects/RatingsSpread.h"

bool GDDLAdvancedLevelInfoPopup::init(const int levelID) {
    if(!FLAlertLayer::init(75)) return false; // that magic number is actually bg opacity btw

    this->levelID = levelID;

    const CCPoint popupSize = {280.0f, 185.0f};
    const auto winSize = CCDirector::sharedDirector()->getWinSize();

    // background
    const auto bg = CCScale9Sprite::create("GJ_square05.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });
    bg->setID("gddl-advanced-level-info-bg"_spr);
    m_mainLayer->addChild(bg, -1);

    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("gddl-advanced-level-info-menu"_spr);
    m_buttonMenu->setContentSize(popupSize);
    m_buttonMenu->setPosition({ winSize.width / 2 - popupSize.x / 2, winSize.height / 2 - popupSize.y / 2});
    m_mainLayer->addChild(m_buttonMenu, 10);
    // close button
    const auto closeButtonSprite = CircleButtonSprite::createWithSpriteFrameName("geode.loader/close.png", .85f,CircleBaseColor::Gray);
    m_closeBtn = CCMenuItemSpriteExtra::create(closeButtonSprite, this, menu_selector(GDDLAdvancedLevelInfoPopup::onClose));
    m_buttonMenu->addChild(m_closeBtn);
    m_closeBtn->setPosition({3.0f, popupSize.y - 3.0f});

    // content

    prepareSearchListener();

    if (RatingsManager::hasSpread(levelID)) {
        addBarCharts();
    } else {
        const auto reqInProgress = CCLabelBMFont::create("Downloading spread...", "chatFont.fnt");
        reqInProgress->setPosition({-100.0f, -40.0f});
        reqInProgress->setID("gddl-advanced-level-info-spread-label"_spr);
        m_buttonMenu->addChild(reqInProgress);
        auto req = web::WebRequest();
        spreadListener.setFilter(req.get(getSpreadEndpointUrl(levelID)));
    }

    return true;
}

void GDDLAdvancedLevelInfoPopup::onClose(cocos2d::CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLAdvancedLevelInfoPopup::prepareSearchListener() {
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

std::string GDDLAdvancedLevelInfoPopup::getSpreadEndpointUrl(const int levelID) {
    return "https://gdladder.com/api/level/" + std::to_string(levelID) + "/submissions/spread";
}


void GDDLAdvancedLevelInfoPopup::addBarCharts() {
    RatingsSpread spread = RatingsManager::getSpread(levelID);
    const auto diffSpreadData = spread.getDiffSpreadData();
    const auto diffChart = BarChartNode::create(diffSpreadData, {150.0f, 20.0f * diffSpreadData.size()}, 50.0f, 20.0f);
    diffChart->setPosition({-230.0f, -150.0f});
    const auto enjSpreadData = spread.getEnjSpreadData();
    const auto enjChart = BarChartNode::create(enjSpreadData, {150.0f, 20.0f * enjSpreadData.size()}, 50.0f, 20.0f);
    enjChart->setPosition({0.0f, -200.0f});
    m_buttonMenu->removeChildByID("gddl-advanced-level-info-spread-label"_spr);
    m_buttonMenu->addChild(diffChart);
    m_buttonMenu->addChild(enjChart);
}

GDDLAdvancedLevelInfoPopup *GDDLAdvancedLevelInfoPopup::create(const int levelID) {
    if (const auto newLayer = new GDDLAdvancedLevelInfoPopup(); newLayer != nullptr && newLayer->init(levelID)) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLAdvancedLevelInfoPopup::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}
