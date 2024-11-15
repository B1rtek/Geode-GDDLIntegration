#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include "Geode/modify/LevelSearchLayer.hpp"

#include "RatingsManager.h"
#include "layers/GDDLSearchLayer.h"

class $modify(GDDLLevelSearchLayer, LevelSearchLayer) {
    // ReSharper disable once CppHidingFunction
    bool init(int p0) {
        if(!LevelSearchLayer::init(p0)) return false;
        GDDLSearchLayer::stopSearch();
        GDDLSearchLayer::restoreValuesAfterSplit();
        addGDDLButton();
        return true;
    }

    void addGDDLButton() {
        const auto tierSprite = CCSprite::create(Mod::get()->expandSpriteName("tier_unrated.png").data());
        tierSprite->setScale(0.235f);
        const auto button = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(GDDLLevelSearchLayer::onGDDLSearch));
        button->setContentSize({30.0f, 30.0f});
        button->setID("gddl_search_button"_spr);
        const auto otherFilterMenu = getChildByIDRecursive("other-filter-menu");
        otherFilterMenu->addChild(button);
        otherFilterMenu->updateLayout();
    }

    void onGDDLSearch(CCObject* sender) { // NOLINT(*-convert-member-functions-to-static)
        GDDLSearchLayer::create()->show();
    }
};