#ifndef GDDLINTEGRATION_GDDLPACKLEVELBROWSER_H
#define GDDLINTEGRATION_GDDLPACKLEVELBROWSER_H

#include "objects/PackInfo.h"
#include <Geode/modify/LevelBrowserLayer.hpp>

using namespace geode::prelude;

struct GDDLPackLevelBrowser : public Modify<GDDLPackLevelBrowser, LevelBrowserLayer> {
    struct Fields {
        int currentPage = 0;
        PackInfo* packInfo = nullptr;
    };

    void handleSearchObject(GJSearchObject* gjSearchObject, const int actualPageNumber);
    void assignPackInfo(PackInfo* packInfo);
};


#endif //GDDLINTEGRATION_GDDLPACKLEVELBROWSER_H