#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(LevelInfoLayer) {

    bool init(GJGameLevel* p0, bool p1) {
        if (!LevelInfoLayer::init(p0, p1)) return false;

        auto starsLabel = m_starsLabel;
        bool isDemon = std::stoi(m_starsLabel->getString()) == 10;
        if(starsLabel && isDemon) {
            auto bmFont = CCLabelBMFont::create("Tier 69", "bigFont.fnt");
            bmFont->setID("gddl-rating"_spr);
            bmFont->setPosition({starsLabel->getPositionX(), starsLabel->getPositionY()}); //193 - 185
            bmFont->setAnchorPoint({0,1});
            // bmFont->setScale(0.325f);
            addChild(bmFont);
            starsLabel->setPositionY(starsLabel->getPositionY() + 10.f);
        }

        return true;
    }
};