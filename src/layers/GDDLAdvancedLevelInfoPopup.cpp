#include "GDDLAdvancedLevelInfoPopup.h"

#include <objects/Skillsets.h>

#include "RatingsManager.h"
#include "objects/RatingsSpread.h"
#include "Utils.h"

bool GDDLAdvancedLevelInfoPopup::init(const int levelID, const std::string& levelName, const std::string& creator) {
    if(!FLAlertLayer::init(75)) return false; // that magic number is actually bg opacity btw

    this->levelID = levelID;
    this->levelName = levelName;
    this->creator = "by " + creator;

    constexpr CCPoint popupSize = {440.0f, 320.0f};
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
    // level name and creator
    const auto levelNameLabel = CCLabelBMFont::create(levelName.c_str(), "bigFont.fnt");
    levelNameLabel->setAnchorPoint({0.0f, 0.5f});
    levelNameLabel->setPosition({30.0f, popupSize.y - 20.0f});
    levelNameLabel->setScale(0.7f);
    m_buttonMenu->addChild(levelNameLabel);
    const auto creatorLabel = CCLabelBMFont::create(creator.c_str(), "goldFont.fnt");
    creatorLabel->setAnchorPoint({0.0f, 0.5f});
    creatorLabel->setPosition({30.0f, popupSize.y - 40.0f});
    creatorLabel->setScale(0.7f);
    m_buttonMenu->addChild(creatorLabel);

    // average ratings and ratings counts
    const auto gddlRating = RatingsManager::getRating(levelID);
    if (gddlRating) {
        addRatingInfo();
    } else {
        const auto stillLoadingLabel = CCLabelBMFont::create("Rating details still loading...", "chatFont.fnt");
        stillLoadingLabel->setAnchorPoint({0.5f, 0.0f});
        stillLoadingLabel->setPosition({popupSize.x / 2, 215.0f});
        stillLoadingLabel->setScale(0.7f);
        stillLoadingLabel->setID("gddl-advanced-level-info-loading-text"_spr);
        m_buttonMenu->addChild(stillLoadingLabel);
    }

    prepareSearchListeners();

    // bar charts
    if (RatingsManager::hasSpread(levelID)) {
        addBarCharts();
    } else {
        const auto reqInProgress = CCLabelBMFont::create("Loading spreads...", "chatFont.fnt");
        reqInProgress->setPosition({popupSize.x / 2, 105.0f});
        reqInProgress->setID("gddl-advanced-level-info-spread-label"_spr);
        m_buttonMenu->addChild(reqInProgress);
        auto req = web::WebRequest();
        spreadListener.setFilter(req.get(getSpreadEndpointUrl(levelID)));
    }

    // skillsets
    if (RatingsManager::hasSkillsets(levelID)) {
        addSkillsets();
    } else {
        const auto reqInProgress = CCLabelBMFont::create("Loading skillsets...", "chatFont.fnt");
        reqInProgress->setPosition({350.0f, 40.0f});
        reqInProgress->setID("gddl-advanced-level-info-skillsets-loading"_spr);
        m_buttonMenu->addChild(reqInProgress);
        auto req = web::WebRequest();
        skillsetsListener.setFilter(req.get(getSkillsetsEndpointUrl(levelID)));
    }

    return true;
}

void GDDLAdvancedLevelInfoPopup::onClose(cocos2d::CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLAdvancedLevelInfoPopup::prepareSearchListeners() {
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

    skillsetsListener.bind([this] (web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            const Skillsets spread = Skillsets(jsonResponse);
            RatingsManager::cacheSkillsets(this->levelID, spread);
            addSkillsets();
        } else if (e->isCancelled()) {
            // :(
        }
    });
}

void GDDLAdvancedLevelInfoPopup::addBarCharts() {
    RatingsSpread spread = RatingsManager::getSpread(levelID);
    float barHeight = 15.0f;
    const auto diffSpreadData = spread.getDiffSpreadData();
    if (diffSpreadData.size() > 11) {
        barHeight = 11.0f / static_cast<float>(diffSpreadData.size()) * barHeight;
    }
    const auto diffChart = BarChartNode::create(diffSpreadData, {150.0f, barHeight * diffSpreadData.size()}, 30.0f, barHeight);
    if (diffSpreadData.size() <= 11) {
        diffChart->setPosition({10.0f, 40.0f + (11 - diffSpreadData.size()) * barHeight});
    } else {
        diffChart->setPosition({10.0f, 40.0f});
    }
    const auto enjSpreadData = spread.getEnjSpreadData();
    const auto enjChart = BarChartNode::create(enjSpreadData, {150.0f, 15.0f * enjSpreadData.size()}, 30.0f, 15.0f);
    enjChart->setPosition({230.0f, 40.0f});
    m_buttonMenu->removeChildByID("gddl-advanced-level-info-spread-label"_spr);
    m_buttonMenu->addChild(diffChart);
    m_buttonMenu->addChild(enjChart);
}

void GDDLAdvancedLevelInfoPopup::addSkillsets() {
    std::vector<std::string> skillsets = RatingsManager::getSkillsets(levelID).getSkillsets();
    std::string skillsetsList = "Top skillsets:\n";
    for (const auto& skillset : skillsets) {
        skillsetsList += '\n' + skillset;
    }
    const auto skillsetsListLabels = TextArea::create(skillsetsList, "chatFont.fnt", 1, 100.0f, {0.5f, 1.0f}, 20, false);
    m_buttonMenu->removeChildByID("gddl-advanced-level-info-skillsets-loading"_spr);
    m_buttonMenu->addChild(skillsetsListLabels);
}

std::string GDDLAdvancedLevelInfoPopup::getSpreadEndpointUrl(const int levelID) {
    return "https://gdladder.com/api/level/" + std::to_string(levelID) + "/submissions/spread";
}

std::string GDDLAdvancedLevelInfoPopup::getSkillsetsEndpointUrl(const int levelID) {
    return "https://gdladder.com/api/level/" + std::to_string(levelID) + "/tags";
}

GDDLAdvancedLevelInfoPopup *GDDLAdvancedLevelInfoPopup::create(const int levelID, const std::string& levelName, const std::string& creator) {
    if (const auto newLayer = new GDDLAdvancedLevelInfoPopup(); newLayer != nullptr && newLayer->init(levelID, levelName, creator)) {
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

void GDDLAdvancedLevelInfoPopup::addRatingInfo() {
    // remove the loading label if it exists
    m_buttonMenu->removeChildByID("gddl-advanced-level-info-loading-text"_spr);
    // add the level info
    const auto gddlRating = RatingsManager::getRating(levelID);
    const auto& info = gddlRating.value();
    std::string ratingText = "Rating: " + (info.rating == -1 ? "N/A" : Utils::floatToString(info.rating, 2));
    if (info.rating != -1) {
        ratingText += " out of " + std::to_string(info.ratingCount) + " submissions";
    }
    std::string enjoymentText = "Enjoyment: " + (info.enjoyment == -1 ? "N/A" : Utils::floatToString(info.enjoyment, 2));
    if (info.enjoyment != -1) {
        enjoymentText += " out of " + std::to_string(info.enjoymentCount) + " submissions";
    }
    const auto ratingLabel = CCLabelBMFont::create(ratingText.c_str(), "chatFont.fnt");
    ratingLabel->setScale(0.7f);
    ratingLabel->setPosition({110.0f, 215.0f});
    m_buttonMenu->addChild(ratingLabel);
    const auto enjoymentLabel = CCLabelBMFont::create(enjoymentText.c_str(), "chatFont.fnt");
    enjoymentLabel->setScale(0.7f);
    enjoymentLabel->setPosition({330.0f, 215.0f});
    m_buttonMenu->addChild(enjoymentLabel);
}
