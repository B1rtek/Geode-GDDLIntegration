#include "GDDLBoomScrollLayer.h"

void GDDLBoomScrollLayer::ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) {
    BoomScrollLayer::ccTouchEnded(p0, p1);
    int actualPage = m_page;
    while (actualPage < 0) {
        actualPage += GDDLRobtopLevelsLayer::Fields::pageCount;
    }
    std::cout << "GDDLBoomScrollLayer::ccTouchEnded" << std::endl;
    if (GDDLRobtopLevelsLayer::Fields::beingBrowsed && Fields::robtopLevelsLayer != nullptr) {
        Fields::robtopLevelsLayer->swiped(actualPage);
    }
}
