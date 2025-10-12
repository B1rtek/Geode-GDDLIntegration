#include "GDDLAdvancedLevelInfoPopup.h"

#include <Geode/ui/LoadingSpinner.hpp>
#include <objects/Skillsets.h>

#include "RatingsManager.h"
#include "objects/RatingsSpread.h"
#include "Utils.h"
#include "GDDLRatingSubmissionLayer.h"

bool GDDLAdvancedLevelInfoPopup::init(GJGameLevel* level, int gddlLevelID) {
    if (!FLAlertLayer::init(75)) return false; // that magic number is actually bg opacity btw

    this->level = level;
    this->gddlLevelID = gddlLevelID;
    this->levelName = level->m_levelName;
    this->creator = gddlLevelID < 10 && level->m_creatorName.empty() ? "RobTop" : level->m_creatorName;

    const auto winSize = CCDirector::sharedDirector()->getWinSize();

    // background
    const auto bg = CCScale9Sprite::create(Utils::getGrayPopupBG().c_str(), {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({winSize.width / 2, winSize.height / 2});
    bg->setID("gddl-advanced-level-info-bg"_spr);
    m_mainLayer->addChild(bg, -1);

    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("gddl-advanced-level-info-menu"_spr);
    m_buttonMenu->setContentSize(popupSize);
    m_buttonMenu->setPosition({winSize.width / 2 - popupSize.x / 2, winSize.height / 2 - popupSize.y / 2});
    m_mainLayer->addChild(m_buttonMenu, 10);
    // close button
    const auto closeButtonSprite = Utils::getGrayPopupCloseButton();
    m_closeBtn = CCMenuItemSpriteExtra::create(closeButtonSprite, this,
                                               menu_selector(GDDLAdvancedLevelInfoPopup::onClose));
    m_buttonMenu->addChild(m_closeBtn);
    m_closeBtn->setPosition({3.0f, popupSize.y - 3.0f});

    // content
    // level name and creator
    levelNameLabel = CCLabelBMFont::create(levelName.c_str(), "bigFont.fnt");
    levelNameLabel->setAnchorPoint({0.0f, 0.5f});
    levelNameLabel->setPosition({70.0f, popupSize.y - 20.0f});
    levelNameLabel->setScale(0.7f);
    if (levelNameLabel->getScaledContentWidth() > 260.0f) {
        levelNameLabel->setScale(260.0f / levelNameLabel->getContentWidth());
    }
    m_buttonMenu->addChild(levelNameLabel);
    const auto creatorLabel = CCLabelBMFont::create(creator.c_str(), "goldFont.fnt");
    creatorLabel->setAnchorPoint({0.0f, 0.5f});
    creatorLabel->setPosition({70.0f, popupSize.y - 40.0f});
    creatorLabel->setScale(0.7f);
    m_buttonMenu->addChild(creatorLabel);
    // buttons - submit
    const auto submitButtonSprite = ButtonSprite::create("Submit rating", "bigFont.fnt", "GJ_button_01.png");
    submitButtonSprite->setScale(0.42f);
    const auto submitButton = CCMenuItemSpriteExtra::create(submitButtonSprite, this,
                                                            menu_selector(GDDLAdvancedLevelInfoPopup::onSubmitClicked));
    submitButton->setPosition({popupSize.x / 2, 22.0f});
    m_buttonMenu->addChild(submitButton);
    // showcase - added in addRatingInfo() because it depends on that actually
    // open in browser
    const auto openInBrowserButtonSprite = ButtonSprite::create("Open in browser", "bigFont.fnt", "GJ_button_02.png");
    openInBrowserButtonSprite->setScale(0.42f);
    const auto openInBrowserButton = CCMenuItemSpriteExtra::create(openInBrowserButtonSprite, this, menu_selector(
            GDDLAdvancedLevelInfoPopup::onOpenInBrowserClicked));
    openInBrowserButton->setPosition({popupSize.x / 2 + 136.0f, 22.0f});
    m_buttonMenu->addChild(openInBrowserButton);

    prepareSearchListeners();

    // average ratings and ratings counts
    const auto gddlRating = RatingsManager::getRating(this->gddlLevelID);
    if (gddlRating) {
        addRatingInfo();
    } else {
        const auto stillLoadingLabel = CCLabelBMFont::create("Rating details loading...", "chatFont.fnt");
        stillLoadingLabel->setAnchorPoint({0.5f, 0.0f});
        stillLoadingLabel->setPosition({popupSize.x / 2, 215.0f});
        stillLoadingLabel->setScale(0.7f);
        stillLoadingLabel->setID("gddl-advanced-level-info-loading-text"_spr);
        m_buttonMenu->addChild(stillLoadingLabel);
        // add gray showcase button
        addShowcaseButton(false);
        // add a ? tier sprite
        addTierSprite(RatingsManager::getDemonTier(this->gddlLevelID));
        auto req = web::WebRequest();
        req.header("User-Agent", Utils::getUserAgent());
        ratingListener.setFilter(req.get(RatingsManager::getRequestUrl(this->gddlLevelID)));
    }

    // bar charts
    if (RatingsManager::hasSpread(this->gddlLevelID)) {
        addBarCharts();
    } else {
        const auto loadingSpinner = LoadingSpinner::create(50.0f);
        loadingSpinner->setPosition({popupSize.x / 2, 122.5f});
        loadingSpinner->setID("gddl-advanced-level-info-spreads-loading"_spr);
        m_buttonMenu->addChild(loadingSpinner);
        auto req = web::WebRequest();
        req.header("User-Agent", Utils::getUserAgent());
        spreadListener.setFilter(req.get(getSpreadEndpointUrl(this->gddlLevelID)));
    }

    // skillsets
    if (RatingsManager::hasSkillsets(this->gddlLevelID)) {
        addSkillsets();
    } else {
        const auto loadingSpinner = LoadingSpinner::create(15.0f);
        loadingSpinner->setPosition({levelNameLabel->getPositionX() + levelNameLabel->getScaledContentWidth() + 10.0f,
                                     popupSize.y - 22.0f});
        loadingSpinner->setID("gddl-advanced-level-info-skillsets-loading"_spr);
        m_buttonMenu->addChild(loadingSpinner);
        auto req = web::WebRequest();
        req.header("User-Agent", Utils::getUserAgent());
        skillsetsListener.setFilter(req.get(getSkillsetsEndpointUrl(this->gddlLevelID)));
    }

    // amazing experiment


    return true;
}

void GDDLAdvancedLevelInfoPopup::onClose(cocos2d::CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLAdvancedLevelInfoPopup::onSkillsetClicked(CCObject *sender) {
    const std::string senderID = dynamic_cast<CCMenuItemSpriteExtra *>(sender)->getID();
    const int start = senderID.find("gddl-advanced-level-info-skillset-") + 34;
    const int end = senderID.size();
    const std::string senderIDStr = senderID.substr(start, end - start);
    const int skillsetID = numFromString<int>(senderIDStr).unwrapOr(0);
    FLAlertLayer::create(Skillsets::skillsetsList[skillsetID].getName().c_str(),
                         Skillsets::skillsetsList[skillsetID].getDescription().c_str(), "OK")->show();
}

void GDDLAdvancedLevelInfoPopup::onSkillsetInfo(CCObject *sender) {
    FLAlertLayer::create("Top Skillsets",
                         "These icons indicate main skillsets of the level, click on them to learn about their meaning.",
                         "OK")->show();
}

void GDDLAdvancedLevelInfoPopup::onSubmitClicked(CCObject *sender) {
    GDDLRatingSubmissionLayer::create(this->level, this->gddlLevelID)->show();
}

void GDDLAdvancedLevelInfoPopup::onShowcaseClicked(CCObject *sender) {
    const auto gddlRating = RatingsManager::getRating(this->gddlLevelID);
    if (gddlRating) {
        const auto &info = gddlRating.value();
        if (info.showcaseVideoID.empty()) {
            Notification::create("This level has no showcase link available", NotificationIcon::Info, 2)->show();
        } else {
            std::string url = "https://youtu.be/" + info.showcaseVideoID;
            web::openLinkInBrowser(url);
        }
    } else {
        Notification::create("Level details haven't loaded yet", NotificationIcon::Warning, 2)->show();
    }

}

void GDDLAdvancedLevelInfoPopup::onOpenInBrowserClicked(CCObject *sender) {
    std::string url = "https://gdladder.com/level/" + std::to_string(this->gddlLevelID);
    web::openLinkInBrowser(url);
}

void GDDLAdvancedLevelInfoPopup::prepareSearchListeners() {
    ratingListener.bind([this](web::WebTask::Event *e) {
        if (web::WebResponse* res = e->getValue()) {
            const std::string response = res->string().unwrapOrDefault();
            if (!RatingsManager::addRatingFromResponse(this->gddlLevelID, response)) {
                const std::string errorMessage = "Error while fetching rating - invalid rating returned";
                Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
                log::error("GDDLAdvancedLevelInfoPopup::ratingListener: {}, ID {}", errorMessage, this->gddlLevelID);
            }
            this->addRatingInfo();
        }
        // TODO add 429 handling maybe?
        else if (e->isCancelled()) {
            const std::string errorMessage = "Error while fetching rating - request cancelled";
            Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
            log::error("GDDLAdvancedLevelInfoPopup::ratingListener: {}, ID: {}", errorMessage, this->gddlLevelID);
        }
    });

    spreadListener.bind([this](web::WebTask::Event *e) {
        if (web::WebResponse *res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            const RatingsSpread spread = RatingsSpread(jsonResponse);
            RatingsManager::cacheSpread(this->gddlLevelID, spread);
            addBarCharts();
        } else if (e->isCancelled()) {
            // TODO error logging
        }
    });

    skillsetsListener.bind([this](web::WebTask::Event *e) {
        if (web::WebResponse *res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            const Skillsets spread = Skillsets(jsonResponse);
            RatingsManager::cacheSkillsets(this->gddlLevelID, spread);
            addSkillsets();
        } else if (e->isCancelled()) {
            // TODO error logging
        }
    });
}

void GDDLAdvancedLevelInfoPopup::addBarCharts() {
    RatingsSpread spread = RatingsManager::getSpread(this->gddlLevelID);
    float barHeight = 15.0f;
    const auto diffSpreadData = spread.getDiffSpreadData();
    if (diffSpreadData.size() > 11) {
        barHeight = 11.0f / static_cast<float>(diffSpreadData.size()) * barHeight;
    }
    const auto diffChart = BarChartNode::create(diffSpreadData, {150.0f, barHeight * diffSpreadData.size()}, 30.0f,
                                                barHeight);
    if (diffSpreadData.size() <= 11) {
        diffChart->setPosition({10.0f, 40.0f + (11 - diffSpreadData.size()) * barHeight});
    } else {
        diffChart->setPosition({10.0f, 40.0f});
    }
    const auto enjSpreadData = spread.getEnjSpreadData();
    const auto enjChart = BarChartNode::create(enjSpreadData, {150.0f, 15.0f * enjSpreadData.size()}, 30.0f, 15.0f);
    enjChart->setPosition({230.0f, 40.0f});
    m_buttonMenu->removeChildByID("gddl-advanced-level-info-spreads-loading"_spr);
    m_buttonMenu->addChild(diffChart);
    m_buttonMenu->addChild(enjChart);
    // separator
    const auto separator = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    separator->setRotation(90.0f);
    separator->setPosition({popupSize.x / 2, 122.5f});
    separator->setScale(0.4f);
    m_buttonMenu->addChild(separator);
}

void GDDLAdvancedLevelInfoPopup::addSkillsets() {
    m_buttonMenu->removeChildByID("gddl-advanced-level-info-skillsets-loading"_spr);
    std::vector<int> skillsets = RatingsManager::getSkillsets(this->gddlLevelID).getSkillsets();
    const float skillsetsStartXPos = levelNameLabel->getPositionX() + levelNameLabel->getScaledContentWidth() + 10.0f;
    for (int i = 0; i < skillsets.size(); i++) {
        const float xPos = skillsetsStartXPos + 20.0f * i;
        if (xPos > popupSize.x - 15.0f) break;
        const auto skillsetButton = CCMenuItemSpriteExtra::create(
                Skillsets::skillsetsList[skillsets[i]].getButtonSprite(), this,
                menu_selector(GDDLAdvancedLevelInfoPopup::onSkillsetClicked));
        skillsetButton->setPosition({xPos, popupSize.y - 22.0f});
        skillsetButton->setID("gddl-advanced-level-info-skillset-" + std::to_string(skillsets[i]));
        m_buttonMenu->addChild(skillsetButton);
    }
    // potential info button
    const float infoButtonXPos = skillsetsStartXPos + 20.0f * skillsets.size();
    if (infoButtonXPos <= popupSize.x - 15.0f && !skillsets.empty()) {
        const auto iButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        iButtonSprite->setScale(0.5f);
        const auto iButton = CCMenuItemSpriteExtra::create(iButtonSprite, this,
                                                           menu_selector(GDDLAdvancedLevelInfoPopup::onSkillsetInfo));
        iButton->setPosition(infoButtonXPos, popupSize.y - 22.0f);
        m_buttonMenu->addChild(iButton);
    }
}

void GDDLAdvancedLevelInfoPopup::addShowcaseButton(bool active) {
    const auto showcaseButtonSprite = ButtonSprite::create("Watch showcase", "bigFont.fnt",
                                                           active ? "GJ_button_06.png" : "GJ_button_04.png");
    showcaseButtonSprite->setScale(0.42f);
    const auto showcaseButton = CCMenuItemSpriteExtra::create(showcaseButtonSprite, this, menu_selector(
            GDDLAdvancedLevelInfoPopup::onShowcaseClicked));
    showcaseButton->setPosition({popupSize.x / 2 - 135.0f, 22.0f});
    showcaseButton->setID("gddl-advanced-level-info-showcase-button"_spr);
    m_buttonMenu->addChild(showcaseButton);
}

void GDDLAdvancedLevelInfoPopup::addTierSprite(const int tier) {
    m_buttonMenu->removeChildByID("gddl-advanced-level-info-tier-button"_spr);
    const auto tierSprite = Utils::getSpriteFromTier(tier);
    tierSprite->setAnchorPoint({0.5f, 0.5f});
    tierSprite->setPosition({45.0f, popupSize.y - 31.0f});
    tierSprite->setID("gddl-advanced-level-info-tier-button"_spr);
    m_buttonMenu->addChild(tierSprite);
}


std::string GDDLAdvancedLevelInfoPopup::getSpreadEndpointUrl(const int levelID) {
    return "https://gdladder.com/api/level/" + std::to_string(levelID) + "/submissions/spread";
}

std::string GDDLAdvancedLevelInfoPopup::getSkillsetsEndpointUrl(const int levelID) {
    return "https://gdladder.com/api/level/" + std::to_string(levelID) + "/tags";
}

GDDLAdvancedLevelInfoPopup *
GDDLAdvancedLevelInfoPopup::create(GJGameLevel *level, int gddlLevelID) {
    if (const auto newLayer = new GDDLAdvancedLevelInfoPopup(); newLayer != nullptr && newLayer->init(level, gddlLevelID)) {
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
    // there's a weird bug on rob's levels layer that causes this stuff to appear in it somehow, idk how, hopefully this fixes it
    if (m_buttonMenu == nullptr) return;
    const auto layer = m_buttonMenu->getParent();
    if (layer == nullptr) return; // no cclayer?? how
    if (layer->getParent() == nullptr) return; // no layer?? how
    if (typeinfo_cast<GDDLAdvancedLevelInfoPopup*>(layer->getParent()) == nullptr) {
        // layer isn't the popup? go away!
        return;
    }
    // remove the loading label if it exists
    m_buttonMenu->removeChildByID("gddl-advanced-level-info-loading-text"_spr);
    // add the level info
    const auto gddlRating = RatingsManager::getRating(this->gddlLevelID);
    if (!gddlRating) {
        const std::string errorMessage = "Error - rating was not returned by the server";
        Notification::create(errorMessage, NotificationIcon::Error)->show();
        log::error("GDDLAdvancedLevelInfoPopup::addRatingInfo: {}, requested ID: {}", errorMessage, this->gddlLevelID);
        return;
    }
    const auto &info = gddlRating.value();
    std::string ratingText = "Rating: " + (info.rating == -1 ? "N/A" : Utils::floatToString(info.rating, 2));
    if (info.rating != -1) {
        ratingText += " out of " + std::to_string(info.ratingCount) + " submissions";
    }
    std::string enjoymentText =
            "Enjoyment: " + (info.enjoyment == -1 ? "N/A" : Utils::floatToString(info.enjoyment, 2));
    if (info.enjoyment != -1) {
        enjoymentText += " out of " + std::to_string(info.enjoymentCount) + " submissions";
    }
    const auto ratingLabel = CCLabelBMFont::create(ratingText.c_str(), "chatFont.fnt");
    ratingLabel->setScale(0.7f);
    ratingLabel->setPosition({110.0f, 215.0f});
    // poor contrast unfortunately
//    if (info.rating != -1) {
//        Utils::recolorTextInLabel(ratingLabel, Utils::floatToString(info.rating, 2),
//                                  RatingsManager::tierColors[info.roundedRating]);
//    }
    m_buttonMenu->addChild(ratingLabel);
    const auto enjoymentLabel = CCLabelBMFont::create(enjoymentText.c_str(), "chatFont.fnt");
    enjoymentLabel->setScale(0.7f);
    enjoymentLabel->setPosition({330.0f, 215.0f});
//    if (info.enjoyment != -1) {
//        Utils::recolorTextInLabel(enjoymentLabel, Utils::floatToString(info.enjoyment, 2),
//                                  RatingsSpread::enjColors[static_cast<int>(std::round(info.enjoyment))]);
//    }
    m_buttonMenu->addChild(enjoymentLabel);
    // handle the showcase button - delete the old one (if it even exists) and add a corrected one
    m_buttonMenu->removeChildByID("gddl-advanced-level-info-showcase-button"_spr);
    addShowcaseButton(!info.showcaseVideoID.empty());
    // correct the "tier button"
    addTierSprite(info.roundedRating);
}
