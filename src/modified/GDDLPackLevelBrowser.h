#ifndef GDDLINTEGRATION_GDDLPACKLEVELBROWSER_H
#define GDDLINTEGRATION_GDDLPACKLEVELBROWSER_H

#include "objects/PackInfo.h"
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/ui/ProgressBar.hpp>
#include <managers/PacksManager.h>
#include <objects/IApiResponseObserver.h>

using namespace geode::prelude;

struct GDDLPackLevelBrowser : public Modify<GDDLPackLevelBrowser, LevelBrowserLayer> {
    struct Fields : public IApiResponseObserver {
        int currentPage = 0;
        int packID = 0;
        bool firstOpen = true;
        ProgressBar* progressBar = nullptr;
        GDDLPackLevelBrowser* m_this;

        Fields() {
            PacksManager::subscribeToObservers(this);
        }

        void updateData() override {
            m_this->updateAfterRefresh();
        }

        ~Fields() override {
            PacksManager::unsubscribeFromObservers(this);
        }
    };

    bool init(GJSearchObject* gjSearchObject);
    gd::string getSearchTitle();
    void loadLevelsFinished(cocos2d::CCArray * p0, char const *p1, int p2) override;
    void onNextPage(CCObject* sender);
    void onPrevPage(CCObject* sender);
    void onRefresh(CCObject* sender);
    void setIDPopupClosed(SetIDPopup* popup, int value) override;
    void onEnter() override;
    void onInfo(CCObject* sender);

    void handleSearchObject(GJSearchObject* gjSearchObject, const int actualPageNumber);
    void assignPackID(int packID);
    void createPackUI();
    void updatePackUI();
    void hideOriginalTextures();
    void updateAfterLoadLevelsFinished();
    void updateAfterRefresh();
    void setCorrectLabelsText();
};


#endif //GDDLINTEGRATION_GDDLPACKLEVELBROWSER_H