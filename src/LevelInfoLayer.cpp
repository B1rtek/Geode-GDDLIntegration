#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/web.hpp>

#include "RatingsManager.h"

using namespace geode::prelude;

class $modify(GDDLInfoLayer, LevelInfoLayer) {
    bool gddlTierUpdated = false;

    bool init(GJGameLevel *p0, bool p1) {
        if (!LevelInfoLayer::init(p0, p1)) return false;

        auto difficultyLabel = m_difficultySprite;
        bool isDemon = std::stoi(m_starsLabel->getString()) == 10;
        if (difficultyLabel && isDemon) {
            gddlTierUpdated = false;
            auto tierLabel = CCLabelBMFont::create("Tier ??", "bigFont.fnt");
            tierLabel->setID("gddl-rating"_spr);
            tierLabel->setPosition({difficultyLabel->getPositionX(), difficultyLabel->getPositionY() + difficultyLabel->getContentSize().height / 2.2f}); // 2.2 :D
            tierLabel->setAnchorPoint({0.5f, 0}) ;
            tierLabel->setScale(0.4f);
            addChild(tierLabel);

            int levelID = m_level->m_levelID;
            int tier = RatingsManager::getDemonTier(levelID);
            if(tier != -1) {
                setTierInformation(tier);
                gddlTierUpdated = true;
            }
        }

        return true;
    }

    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();
        auto starsLabel = m_starsLabel;
        bool isDemon = std::stoi(m_starsLabel->getString()) == 10;
        if (!starsLabel || !isDemon || gddlTierUpdated) return;

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
                setTierInformation(tier);
            })
            .expect([this](std::string const& error) {
                setTierInformation(-1);
            });
        }
    }

    void setTierInformation(int tier) {
        std::string tierString = ":(";
        if (tier != -1) {
            tierString = std::to_string(tier);
        }
        std::string tierLabelText = "Tier " + tierString;
        auto tierLabel = typeinfo_cast<CCLabelBMFont*>(getChildByID("gddl-rating"_spr));
        tierLabel->setString(tierLabelText.c_str());
        tierLabel->setColor(RatingsManager::getTierColor(tier));
    }
};
