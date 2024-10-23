#include <Geode/Bindings.hpp>
#include <Geode/modify/BoomScrollLayer.hpp>
#include "LevelSelectLayer.cpp"

using namespace geode::prelude;

class $modify(GDDLBoomScrollLayer, BoomScrollLayer) {

    void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) override {
        BoomScrollLayer::ccTouchEnded(p0, p1);
        int actualPage = m_page;
        while (actualPage < 0) {
            actualPage += GDDLRobtopLevelsLayer::Fields::pageCount;
        }
        if (GDDLRobtopLevelsLayer::Fields::beingBrowsed) {
            const auto robtopLevelsLayer = dynamic_cast<GDDLRobtopLevelsLayer*>(getParent());
            robtopLevelsLayer->swiped(actualPage);
        }
    }
};