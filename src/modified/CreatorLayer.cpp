#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <layers/GDDLHomeLayer.h>
#include <layers/GDDLPacksLayer.h>
#include <layers/GDDLSearchLayerV2.h>
#include <layers/GDDLThemeListLayer.h>

class $modify(GDDLCreatorLayer, CreatorLayer) {

    bool init() {
        if (!CreatorLayer::init()) return false;
        addGDDLButton();
        return true;
    }

    void addGDDLButton() {
        const auto tierSprite = cocos2d::CCSprite::create(geode::Mod::get()->expandSpriteName("tier_unrated.png").data());
        tierSprite->setScale(0.235f);
        const auto button = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(GDDLCreatorLayer::onGDDLHome));
        button->setContentSize({30.0f, 30.0f});
        button->setID("gddl_menu_button"_spr);
        const auto otherFilterMenu = getChildByIDRecursive("top-right-menu");
        otherFilterMenu->addChild(button);
        otherFilterMenu->updateLayout();
    }

    void onGDDLHome(CCObject* sender) { // NOLINT(*-convert-member-functions-to-static)
        GDDLPacksLayer::scene();
    }
};