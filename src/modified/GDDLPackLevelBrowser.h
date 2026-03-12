#ifndef GDDLINTEGRATION_GDDLPACKLEVELBROWSER_H
#define GDDLINTEGRATION_GDDLPACKLEVELBROWSER_H

#include "objects/PackInfo.h"
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/ui/ProgressBar.hpp>

using namespace geode::prelude;

struct GDDLPackLevelBrowser : public Modify<GDDLPackLevelBrowser, LevelBrowserLayer> {
    struct Fields {
        int currentPage = 0;
        PackInfo* packInfo = nullptr;
        bool firstOpen = true;
        ProgressBar* progressBar = nullptr;
    };

    gd::string getSearchTitle();
    void loadLevelsFinished(cocos2d::CCArray * p0, char const *p1, int p2) override;
    void onNextPage(CCObject* sender);
    void onPrevPage(CCObject* sender);
    void setIDPopupClosed(SetIDPopup* popup, int value) override;
    void keyBackClicked() override;
    void onBack(cocos2d::CCObject* sender) override;
    void onEnterTransitionDidFinish() override;

    void backActions();
    void handleSearchObject(GJSearchObject* gjSearchObject, const int actualPageNumber);
    void assignPackInfo(PackInfo* packInfo);
    void createPackUI();
    void updatePackUI();
    void updateAfterLoadLevelsFinished();
    void setCorrectLabelsText();
};


#endif //GDDLINTEGRATION_GDDLPACKLEVELBROWSER_H