#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(LevelInfoLayer) {
    bool init(GJGameLevel *p0, bool p1) {
        if (!LevelInfoLayer::init(p0, p1)) return false;

        auto starsLabel = m_starsLabel;
        bool isDemon = std::stoi(m_starsLabel->getString()) == 10;
        if (starsLabel && isDemon) {
            auto tierLabel = CCLabelBMFont::create("Tier 69", "bigFont.fnt");
            tierLabel->setID("gddl-rating"_spr);
            float labelShift = starsLabel->getContentSize().height / 2.0f;
            tierLabel->setPosition({starsLabel->getPositionX(), starsLabel->getPositionY() - labelShift}); //160x185
            tierLabel->setAnchorPoint({0.5f, 0.5f});
            tierLabel->setScale(0.4f);
            addChild(tierLabel);
        }

        return true;
    }
};
