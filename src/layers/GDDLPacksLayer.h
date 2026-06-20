#ifndef GDDLINTEGRATION_GDDLPACKSLAYER_H
#define GDDLINTEGRATION_GDDLPACKSLAYER_H

#include <Geode/Bindings.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/web.hpp>
#include <objects/IApiResponseObserver.h>
#include <objects/PackCategoryInfo.h>
#include <objects/PackInfo.h>

#include "GDDLThemeListLayer.h"

using namespace geode::prelude;

class GDDLPacksLayer : public GDDLThemeListLayer, public IApiResponseObserver {
    CCLabelBMFont* titleLabel = nullptr;
    int page = 1;
    TaskHolder<web::WebResponse> packsTaskHolder;

    bool init() override;

    void createListFrame();
    void updateDisplay();
    void onNextPage(CCObject* sender);
    void onPrevPage(CCObject* sender);
    void onBack(CCObject* sender);
    void onRefresh(CCObject* sender);
    void keyBackClicked() override;
    void backActions() override;
public:
    static GDDLPacksLayer* create();
    static GDDLPacksLayer* scene();
    
    void updateData() override;
};


#endif //GDDLINTEGRATION_GDDLPACKSLAYER_H