#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include "Geode/modify/LevelSearchLayer.hpp"

#include <layers/GDDLSearchLayerV2.h>

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

        const auto tierSprite2 = CCSprite::create(Mod::get()->expandSpriteName("tier_unrated.png").data());
        tierSprite2->setScale(0.235f);
        tierSprite2->setColor(ccc3(255, 0, 0));
        const auto button2 = CCMenuItemSpriteExtra::create(tierSprite2, this, menu_selector(GDDLLevelSearchLayer::onGDDLSearchV2));
        button->setContentSize({30.0f, 30.0f});
        button->setID("gddl_search_button"_spr);
        otherFilterMenu->addChild(button2);
        otherFilterMenu->updateLayout();
    }

    void onGDDLSearch(CCObject* sender) { // NOLINT(*-convert-member-functions-to-static)
        GDDLSearchLayer::create()->show();
    }

    void onGDDLSearchV2(CCObject* sender) { // NOLINT(*-convert-member-functions-to-static)
        GDDLSearchLayerV2::create()->show();
    }
};