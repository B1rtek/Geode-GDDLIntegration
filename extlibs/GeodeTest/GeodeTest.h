#ifndef GEODETEST_H
#define GEODETEST_H

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <simpletest.h>

using namespace geode::prelude;

DEFINE_TEST_G(TestMessageSpace, Basic)
{
    bool found = false;
    for (TestFixture* i = TestFixture::GetFirstTest(); i; i = i->GetNextTest())
    {
        if (strcmp(i->TestName(), "MaxErrors") == 0 && strcmp(i->TestGroup(), "MaxErrors") == 0)
        {
            i->ExecuteTest();

            int errorCount = 0;
            for (TestError const* err = i->GetFirstError(); err != i->GetLastError(); err = err->next)
            {
                ++errorCount;
            }

            TEST_EQ(i->NumErrors(), i->NumTests());
            TEST_EQ(i->NumErrors(), errorCount + 1);

            found = true;
            break;
        }
    }

    TEST(found);
}

class $modify(GeodeTest, MenuLayer) {
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


#endif //GEODETEST_H
