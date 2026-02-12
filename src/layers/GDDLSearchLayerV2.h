#ifndef GDDLINTEGRATION_GDDLSEARCHLAYERV2_H
#define GDDLINTEGRATION_GDDLSEARCHLAYERV2_H

#include <Geode/Bindings.hpp>
#include <Geode/utils/web.hpp>
#include <nodes/SearchLayerPage.h>

using namespace geode::prelude;

class GDDLSearchLayerV2 : public FLAlertLayer {
    constexpr static CCPoint popupSize = {440.0f, 290.0f};

    int currentPageNumber = 0;
    SearchLayerPage* currentPage = nullptr;

    // normal popup stuff
    bool init() override;
    void onClose(CCObject *sender);

    // internal ui methods
    void createBaseUI();
    void displayPage(int pageNumber);
public:
    // normal popup stuff
    static GDDLSearchLayerV2* create();
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLSEARCHLAYERV2_H