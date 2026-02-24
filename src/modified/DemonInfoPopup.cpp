#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/modify/DemonInfoPopup.hpp>
#include <layers/GDDLDemonSplitLayer.h>

using namespace geode::prelude;

class $modify(GDDLDemonInfoPopup, DemonInfoPopup) {
    bool init(int easyClassic, int mediumClassic, int hardClassic, int insaneClassic, int extremeClassic, int easyPlatformer,
        int mediumPlatformer, int hardPlatformer, int insanePlatformer, int extremePlatformer, int weekly, int gauntlet) {
        if (!DemonInfoPopup::init(easyClassic, mediumClassic, hardClassic, insaneClassic, extremeClassic, easyPlatformer, mediumPlatformer, hardPlatformer, insanePlatformer, extremePlatformer, weekly, gauntlet)) {
            return false;
        }
        auto blMenu = m_mainLayer->getChildByID("bottom-left-menu");
        // create GDDL button
        auto gddlButtonSprite = CCSprite::create(Mod::get()->expandSpriteName(std::string_view{"tier_unrated.png"}).data());
        gddlButtonSprite->setScale(0.15f);
        auto gddlButton = CCMenuItemSpriteExtra::create(gddlButtonSprite, this, menu_selector(GDDLDemonInfoPopup::onGDDLDemonSplit));
        gddlButton->setScale(0.5f);
        gddlButton->setContentSize({20.0f, 20.0f});
        gddlButton->setID("gddl-button");
        blMenu->addChild(gddlButton);
        blMenu->updateLayout();

        return true;
    }

    void onGDDLDemonSplit(CCObject* sender) { // NOLINT(*-convert-member-functions-to-static)
        GDDLDemonSplitLayer::create()->show();
    }
};