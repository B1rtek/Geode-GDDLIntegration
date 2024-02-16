#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>

#include "RatingsManager.h"
#include "GDDLSearchLayer.h"

class $modify(GDDLLevelSearchLayer, LevelSearchLayer) {
    bool init(int p0) {
        if(!LevelSearchLayer::init(p0)) return false;
        GDDLSearchLayer::stopSearch();
        addGDDLButton();
        return true;
    }

    void addGDDLButton() {
        std::string textureName = Mod::get()->expandSpriteName("tier_unrated.png");
        auto tierSprite = CCSprite::create(textureName.c_str());
        tierSprite->setScale(0.235f);
        auto button = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(GDDLLevelSearchLayer::onGDDLSearch));
        button->setContentSize({30.0f, 30.0f});
        button->setID("gddl_search_button"_spr);
        getChildByIDRecursive("other-filter-menu")->addChild(button);
        auto buttonAbove = getChildByIDRecursive("lists-button");
        button->setPosition({buttonAbove->getPositionX(), buttonAbove->getPositionY()-(50.0f * (Mod::get()->getSettingValue<int64_t>("move-gddl-search-button-down") + 1))});
    }

    void onGDDLSearch(CCObject* sender) {
        GDDLSearchLayer::create()->show();
    }
};