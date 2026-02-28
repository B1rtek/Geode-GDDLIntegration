#ifndef GDDLINTEGRATION_GDDLBROWSERLAYER_H
#define GDDLINTEGRATION_GDDLBROWSERLAYER_H

#include <Geode/Bindings.hpp>
#include "Geode/modify/LevelBrowserLayer.hpp"

struct GDDLLevelBrowserLayer : public geode::Modify<GDDLLevelBrowserLayer, LevelBrowserLayer> {
    struct Fields {
        int currentPage = 0;
    };

    bool init(GJSearchObject * p0);
    void loadLevelsFinished(cocos2d::CCArray * p0, char const *p1, int p2) override;
    void onNextPage(CCObject* sender);
    void onPrevPage(CCObject* sender);
    // void onGoToPage(CCObject* sender);
    void setCorrectLabelsText();
    void handleSearchObject(GJSearchObject * searchObject, int resultsCount);
};

#endif //GDDLINTEGRATION_GDDLBROWSERLAYER_H