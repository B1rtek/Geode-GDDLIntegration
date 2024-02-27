/**
 * Include the Geode headers.
 */
// ReSharper disable once CppUnusedIncludeDirective
#include <Geode/Geode.hpp>
/**
 * Required to modify the MenuLayer class
 */
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>


#include "GDDLSearchLayer.h"
#include "RatingsManager.h"

/**
 * Brings cocos2d and all Geode namespaces
 * to the current scope.
 */
using namespace geode::prelude;

#include <simpletest/simpletest.h>

DEFINE_TEST_G(TestBool, Basic) {
    bool truth = true;
    bool lie = false;
    TEST(truth);
    TEST(!lie);
    TEST(!!truth);
}

DEFINE_TEST_G(TestGeode, Basic) {
    GJSearchObject* searchObject = GDDLSearchLayer::getSearchObjectForPage(1);
    TEST_EQ(searchObject->m_searchType, SearchType::Type19);
}

class $modify(GDDLTestLayer, MenuLayer) {

    bool init() override {
        if(!MenuLayer::init()) return false;
        addTestButton();
        return true;
    }

    void addTestButton() {
        auto tierSprite = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
        auto button = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(GDDLTestLayer::onTest));
        button->setContentSize({25.0f, 25.0f});
        button->setID("geode_test_button"_spr);
        CCMenu* rightSideMenu = dynamic_cast<CCMenu *>(getChildByIDRecursive("right-side-menu"));
        rightSideMenu->addChild(button, 1);
        rightSideMenu->updateLayout();
    }

    void onTest(CCObject* sender)  {
        std::string result = "not owo";
        if(TestFixture::ExecuteAllTests()) {
            result = "Pass";
        }
        log::debug("Test results: {}", result);
    }
};