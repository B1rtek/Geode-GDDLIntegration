#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/web.hpp>

#include "RatingsManager.h"
#include "Utils.h"

using namespace geode::prelude;

class $modify(GDDLInfoLayer, LevelInfoLayer) {
    bool gddlTierUpdated = false;

    bool init(GJGameLevel *p0, bool p1) {
        if (!LevelInfoLayer::init(p0, p1)) return false;

        auto starsLabel = m_starsLabel;
        bool isDemon = std::stoi(m_starsLabel->getString()) == 10;
        if (starsLabel && isDemon) {
            m_fields->gddlTierUpdated = false;
            bool moveToLevelName = Mod::get()->getSettingValue<bool>("move-button-to-level-name");
            int levelNamePos = Mod::get()->getSettingValue<int64_t>("pos-next-to-level-name");

            CCPoint menuPosition, buttonPosition;
            CCSize menuSize;
            float buttonScale = 1.0f;
            if(moveToLevelName && levelNamePos != 0) {
                const auto levelNameLabel = typeinfo_cast<CCLabelBMFont *>(getChildByID("title-label"));
                const auto levelNamePosition = levelNameLabel->getPosition();
                const auto levelNameSize = levelNameLabel->getContentSize();
                if (levelNamePos > 0) { // right
                    menuPosition = CCPoint{levelNamePosition.x + levelNameSize.width/2.5f, levelNamePosition.y - levelNameSize.height/2.25f};
                } else { // left
                    menuPosition = CCPoint{levelNamePosition.x - levelNameSize.width/2.5f - 25.0f, levelNamePosition.y - levelNameSize.height/2.25f};
                }
                menuSize = CCSize{25, 25};
                buttonPosition = CCPoint{12.5f, 12.5f};
                buttonScale = 0.5f;
            } else {
                const auto diffPosition = m_difficultySprite->getPosition();
                const auto diffSize = m_difficultySprite->getContentSize();
                menuPosition = CCPoint{diffPosition.x - 50 - diffSize.width/2, diffPosition.y - diffSize.height/3.2f};
                menuSize = CCSize{50, 50};
                buttonPosition = CCPoint{25, 25};
            }
            placeGDDLButton(menuPosition, menuSize, buttonPosition, buttonScale);

            int levelID = m_level->m_levelID;
            int tier = RatingsManager::getDemonTier(levelID);
            if(tier != -1) {
                updateButton(tier);
                m_fields->gddlTierUpdated = true;
            }
        }

        return true;
    }

    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();
        auto starsLabel = m_starsLabel;
        bool isDemon = std::stoi(m_starsLabel->getString()) == 10;
        if (!starsLabel || !isDemon || m_fields->gddlTierUpdated) return;

        // fetch information
        retain();
        int levelID = m_level->m_levelID;
        int tier = RatingsManager::getDemonTier(levelID);
        if (tier == -1) {
            web::AsyncWebRequest()
            .fetch(RatingsManager::getRequestUrl(levelID))
            .text()
            .then([this, levelID](std::string const& response) {
                int tier = -1;
                if(RatingsManager::addRatingFromResponse(levelID, response)) {
                    tier = RatingsManager::getDemonTier(levelID);
                }
                updateButton(tier);
                release();
            })
            .expect([this](std::string const& error) {
                updateButton(-1);
                release();
            });
        }
    }

    void placeGDDLButton(const CCPoint& menuPosition, const CCSize& menuSize, const CCPoint& buttonPosition, float buttonScale = 1.0f) {
        auto menu = CCMenu::create();
        menu->setPosition(menuPosition);
        menu->setContentSize(menuSize);
        menu->setScale(buttonScale);
        menu->setID("rating-menu"_spr);
        addChild(menu);

        auto button = CCMenuItemSpriteExtra::create(getDefaultSprite(), this, menu_selector(GDDLInfoLayer::onGDDLInfo));
        button->setPosition(buttonPosition);
        button->setID("rating"_spr);
        menu->addChild(button);
    }

    void updateButton(int tier) {
        auto menu = typeinfo_cast<CCMenu*>(getChildByID("rating-menu"_spr));
        if (!menu) return;
        auto tierButton = typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("rating"_spr));
        if (!tierButton) return;
        tierButton->removeAllChildren();

        auto tierSprite = getSpriteFromTier(tier);
        if (tierSprite) {
            tierButton->addChild(tierSprite);
        } else {
            tierButton->addChild(getDefaultSprite());
        }
    }

    CCSprite *getTierSpriteFromName(const char *name) {
        auto textureName = Mod::get()->expandSpriteName(name);
        auto sprite = CCSprite::create(textureName);
        if (!sprite) return getDefaultSprite();

        sprite->setScale(0.275f);
        sprite->setAnchorPoint({0, 0});

        return sprite;
    }

    CCSprite *getSpriteFromTier(int tier) {
        if (tier == -1) return getDefaultSprite();
        return getTierSpriteFromName(("tier_" + std::to_string(tier) + ".png").c_str());
    }

    CCSprite *getDefaultSprite() {
        return getTierSpriteFromName("tier_unrated.png");
    }

    void onGDDLInfo(CCObject *sender) {
        auto rating = RatingsManager::getRating(m_level->m_levelID);
        if (!rating) return;
        auto info = rating.value();

        std::string tier = info.rating == -1 ? "N/A" : Utils::floatToString(info.rating, 2);
        std::string enjoyment = info.enjoyment == -1 ? "N/A" : Utils::floatToString(info.enjoyment, 2);
        std::string submissionCount = info.submissionCount == -1 ? "N/A" : std::to_string(info.submissionCount);

        auto layer = FLAlertLayer::create("GDDL Information", "Tier: " + tier + "\nEnjoyment: " + enjoyment + "\nTotal submissions: " + submissionCount, "Close");
        layer->show();
    }
};
