#include <Geode/Bindings.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(GDDLProfileMod, ProfilePage) {
    virtual TodoReturn loadPageFromUserInfo(GJUserScore* p0) {
        ProfilePage::loadPageFromUserInfo(p0);
        auto statsMenu = m_mainLayer->getChildByID("stats-menu");
        auto demonButton = m_mainLayer->getChildByIDRecursive("demons-icon");
        // create GDDL button
        std::string gddlButtonSpritePath = Mod::get()->expandSpriteName("tier_unrated.png");
        auto gddlButtonSprite = CCSprite::create(gddlButtonSpritePath.c_str());
        gddlButtonSprite->setScale(0.15f);
        auto gddlButton = CCMenuItemSpriteExtra::create(gddlButtonSprite, this, menu_selector(GDDLProfileMod::test));
        gddlButton->setScale(0.5f);
        gddlButton->setContentSize({20.0f, 20.0f});
        gddlButton->setID("gddl-button");
        this->m_buttons->addObject(gddlButton);
        // create CCNode layout for the demon and gddl buttons
        CCMenu* demonButtonsContainer = CCMenu::create();
        statsMenu->removeChild(demonButton);
        demonButtonsContainer->addChild(demonButton);
        demonButtonsContainer->addChild(gddlButton);
        demonButtonsContainer->reorderChild(demonButton, 2);
        demonButtonsContainer->reorderChild(gddlButton, 3);
        demonButtonsContainer->setLayout(RowLayout::create()->setGap(3.0f));
        demonButtonsContainer->setContentSize({50.0f, 30.0f});
        statsMenu->addChild(demonButtonsContainer);
        demonButtonsContainer->updateLayout();
        statsMenu->reorderChild(demonButtonsContainer, 1); // who cares it should be fine as long as it's over 0
        statsMenu->updateLayout();
        gddlButtonSprite->setPosition({gddlButtonSprite->getParent()->getContentSize().width/2, gddlButtonSprite->getParent()->getContentSize().height/2});
    }

    void test(CCObject* sender) {
        FLAlertLayer::create("did it work?", "yes", "very cool ig")->show();
    }
};