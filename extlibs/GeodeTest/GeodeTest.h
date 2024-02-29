#ifndef GEODETEST_H
#define GEODETEST_H

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "simpletest/simpletest.h"

using namespace geode::prelude;

class $modify(GeodeTest, MenuLayer) {
    bool init() override {
        if(!MenuLayer::init()) return false;
        addTestButton();
        return true;
    }

    void addTestButton() {
        auto tierSprite = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
        auto button = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(GeodeTest::onTest));
        button->setContentSize({25.0f, 25.0f});
        button->setID("geode_test_button"_spr);
        CCMenu* rightSideMenu = dynamic_cast<CCMenu *>(getChildByIDRecursive("right-side-menu"));
        rightSideMenu->addChild(button, 1);
        rightSideMenu->updateLayout();
    }

    void onTest(CCObject* sender)  {
        TestFixture::ExecuteAllTests(nullptr, nullptr, TestFixture::Verbose);
    }
};


#endif //GEODETEST_H
