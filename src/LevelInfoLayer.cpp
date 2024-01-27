#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

#include "RatingsManager.h"

using namespace geode::prelude;

class $modify(LevelInfoLayer) {
    bool init(GJGameLevel *p0, bool p1) {
        if (!LevelInfoLayer::init(p0, p1)) return false;

        auto starsLabel = m_starsLabel;
        bool isDemon = std::stoi(m_starsLabel->getString()) == 10;
        if (starsLabel && isDemon) {
            auto tierLabel = CCLabelBMFont::create("Tier ??", "bigFont.fnt");
            tierLabel->setID("gddl-rating"_spr);
            float labelShiftRows = m_level->m_coins > 0 ? 2.0f : 1.0f;
            float labelShift = starsLabel->getContentSize().height * 0.9f / (2.0f / labelShiftRows); // I'm really trying to make it perfect
            tierLabel->setPosition({starsLabel->getPositionX(), starsLabel->getPositionY() - labelShift}); //160x185
            tierLabel->setAnchorPoint({0.5f, 0.5f});
            tierLabel->setScale(0.4f);
            addChild(tierLabel);

            // fetch information
            int tier = RatingsManager::getDemonTier(m_level->m_levelID);
            std::string tierString = ":(";
            if (tier != -1) {
                tierString = std::to_string(tier);
            }
            std::string tierLabelText = "Tier " + tierString;
            tierLabel->setString(tierLabelText.c_str());
            tierLabel->setColor(RatingsManager::getTierColor(tier));
        }

        return true;
    }
};
