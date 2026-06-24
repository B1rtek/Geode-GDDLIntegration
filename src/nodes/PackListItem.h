#ifndef GDDLINTEGRATION_PACKLISTITEM_H
#define GDDLINTEGRATION_PACKLISTITEM_H

#include <Geode/Bindings.hpp>
#include <Geode/ui/ProgressBar.hpp>
#include <objects/IApiResponseObserver.h>
#include <objects/PackInfo.h>

using namespace geode::prelude;

class PackListItem : public CCNode, public IApiResponseObserver {
    static constexpr float itemHeight = 50.0f;

    int packID;
    ProgressBar* progressBar = nullptr;

    bool init(const float width, const int packID, bool dark);
    void updateProgressbar();

    void onView(CCObject* sender);
    void forwardToLevelBrowser(GJSearchObject* gjSearchObject);
public:
    static PackListItem* create(const float width, const int packID, bool dark);
    void draw() override;

    void updateData() override;
};


#endif //GDDLINTEGRATION_PACKLISTITEM_H