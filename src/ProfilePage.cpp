#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/modify/ProfilePage.hpp>

#include "GDDLDemonSplitLayer.h"

using namespace geode::prelude;

class $modify(GDDLProfileMod, ProfilePage) {
    virtual TodoReturn loadPageFromUserInfo(GJUserScore* p0) {
        ProfilePage::loadPageFromUserInfo(p0);
        if(p0->m_accountID != GJAccountManager::sharedState()->m_accountID) return;
        // get the existing nodes
        auto statsMenu = m_mainLayer->getChildByID("stats-menu");
        // create GDDL button
        std::string gddlButtonSpritePath = Mod::get()->expandSpriteName("tier_unrated.png");
        auto gddlButtonSprite = CCSprite::create(gddlButtonSpritePath.c_str());
        gddlButtonSprite->setScale(0.15f);
        auto gddlButton = CCMenuItemSpriteExtra::create(gddlButtonSprite, this, menu_selector(GDDLProfileMod::onGDDLDemonSplit));
        gddlButton->setScale(0.5f);
        gddlButton->setContentSize({20.0f, 20.0f});
        gddlButton->setID("gddl-button");
        this->m_buttons->addObject(gddlButton);
        // remove the demon button from the original layout and put it in the new one
        statsMenu->addChild(gddlButton);
        statsMenu->reorderChild(gddlButton, 1);
        // magic that makes it look properly
        statsMenu->updateLayout();
        gddlButtonSprite->setPosition({gddlButtonSprite->getParent()->getContentSize().width/2, gddlButtonSprite->getParent()->getContentSize().height/2});
    }

    void onGDDLDemonSplit(CCObject* sender) {
        GDDLDemonSplitLayer::create()->show();
    }
};