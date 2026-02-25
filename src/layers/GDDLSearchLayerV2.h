#ifndef GDDLINTEGRATION_GDDLSEARCHLAYERV2_H
#define GDDLINTEGRATION_GDDLSEARCHLAYERV2_H

#include <Geode/Bindings.hpp>
#include <Geode/utils/web.hpp>
#include <nodes/SearchLayerPage.h>
#include <objects/SearchObject.h>

using namespace geode::prelude;

class GDDLSearchLayerV2 : public FLAlertLayer {
    constexpr static CCPoint popupSize = {440.0f, 290.0f};
    const inline static std::vector<std::string> pageNames = {"Simplified search", "Page 1", "Page 2", "Page 3"};

    int currentPageNumber = 1;
    TextInput* levelNameTextInput = nullptr;
    CCMenuItemSpriteExtra* searchButton = nullptr;
    CCMenuItemSpriteExtra* resetButton = nullptr;
    CCLabelBMFont* pageNumberLabel = nullptr;
    SearchLayerPage* currentPage = nullptr;
    SearchObject searchObject = SearchObject();

    // normal popup stuff
    bool init() override;
    void onClose(CCObject *sender);

    // internal ui methods
    void createBaseUI();
    void displayPage(int pageNumber);
    void updatePageNumberLabel();

    // callbacks
    void onSearchClicked(CCObject* sender);
    void onResetClicked(CCObject* sender);
    void onPageLeftClicked(CCObject* sender);
    void onPageRightClicked(CCObject* sender);
public:
    // normal popup stuff
    static GDDLSearchLayerV2* create();
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLSEARCHLAYERV2_H