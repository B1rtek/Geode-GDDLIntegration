#include "GDDLSearchLayerV2.h"

#include <Utils.h>
#include <Geode/utils/cocos.hpp>
#include <nodes/searchcontrols/EnumInputControl.h>
#include <nodes/searchcontrols/TextInputControl.h>
#include <nodes/searchcontrols/RangeInputControl.h>
#include <nodes/searchcontrols/CheckboxInputControl.h>

bool GDDLSearchLayerV2::init() {
    if (!FLAlertLayer::init(150))
        return false;

    createBaseUI();
    searchObject.loadSettings();
    levelNameTextInput->setString(searchObject.getLevelNameSetting()->getSettingValue());
    displayPage(currentPageNumber);

    return true;
}

void GDDLSearchLayerV2::createBaseUI() {
    // window size and buttons boilerplate
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    // background
    const auto bg = CCScale9Sprite::create(Utils::getGrayPopupBG().c_str(), {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({winSize.width / 2, winSize.height / 2});
    bg->setID("gddl-demon-search-popup"_spr);
    m_mainLayer->addChild(bg, -1);
    const float aspectRatioFixX = (569.0f - winSize.width) / 2;
    const float aspectRatioFixY = (320.0f - winSize.height) / 2;
    // menu with the main layout
    m_buttonMenu = CCMenu::create();
    m_buttonMenu->setContentSize({winSize.width, winSize.height});
    m_buttonMenu->setPosition({64.5f - aspectRatioFixX, 295.0f - aspectRatioFixY});
    m_buttonMenu->setID("gddl-demon-search-menu"_spr);
    m_mainLayer->addChild(m_buttonMenu, 0);

    // due to touch prio, we have to recreate everything, I could also probably create a separate menu...
    recreateMainButtonMenu();

    // initialize page
    currentPage = SearchLayerPage::create();
    currentPage->setPosition({winSize.width / 2 - popupSize.x/2 + 2.5f, winSize.height / 2 - popupSize.y/2 + 40.0f});
    m_mainLayer->addChild(currentPage);
}

void GDDLSearchLayerV2::recreateMainButtonMenu() {
    // clear
    m_buttonMenu->removeAllChildren();

    // close button
    const auto closeButtonSprite = Utils::getGrayPopupCloseButton();
    const auto closeButton =
            CCMenuItemSpriteExtra::create(closeButtonSprite, this, menu_selector(GDDLSearchLayerV2::onClose));
    m_buttonMenu->addChild(closeButton, 1);
    closeButton->setPosition({10.0f, 0.0f});
    // title
    const auto title = CCLabelBMFont::create("GDDL Search", "goldFont.fnt");
    m_buttonMenu->addChild(title, 1);
    title->setPosition({220.0f, -10.0f});
    title->setScale(0.7f);
    title->setID("gddl-demon-search-title"_spr);

    // base controls - level name input field, search and clear buttons, page switching buttons, page number label
    // level name input field
    Utils::createGeodeTextInput(m_buttonMenu, levelNameTextInput, "bigFont.fnt", "Enter a level name...", 64, {310.0f, 30.0f}, {175.0f, -45.0f});
    // search button
    const auto searchButtonSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn06_001.png");
    searchButton = CCMenuItemSpriteExtra::create(searchButtonSprite, this, menu_selector(GDDLSearchLayerV2::onSearchClicked));
    m_buttonMenu->addChild(searchButton, 1);
    searchButton->setPosition({357.5f, -45.0f});
    // reset/clear button
    const auto resetButtonSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn07_001.png");
    resetButton = CCMenuItemSpriteExtra::create(resetButtonSprite, this, menu_selector(GDDLSearchLayerV2::onResetClicked));
    m_buttonMenu->addChild(resetButton, 1);
    resetButton->setPosition({402.5f, -45.0f});
    // page number label
    pageNumberLabel = CCLabelBMFont::create("somethingverylongsoittakesupalloftheavailablespace", "bigFont.fnt");
    Utils::scaleLabelToWidth(pageNumberLabel, 110.0f);
    m_buttonMenu->addChild(pageNumberLabel, 1);
    pageNumberLabel->setPosition({popupSize.x/2, -255.0f});
    // buttons around the label
    Utils::createLeftRightButtonsAround(pageNumberLabel, {13.0f, 19.0f}, this, menu_selector(GDDLSearchLayerV2::onPageLeftClicked), menu_selector(GDDLSearchLayerV2::onPageRightClicked));
}

void GDDLSearchLayerV2::reloadAfterSwitchingPage() {
    currentPage->saveSettings();
    searchObject.getLevelNameSetting()->setSettingValue(this->levelNameTextInput->getString());
    searchObject.saveSettings();
    recreateMainButtonMenu();
    searchObject.loadSettings();
    levelNameTextInput->setString(searchObject.getLevelNameSetting()->getSettingValue());
    displayPage(currentPageNumber);
}

void GDDLSearchLayerV2::displayPage(int pageNumber) {
    currentPage->clearContent();
    if (pageNumber == 0) {
        // simplified search page

    } else if (pageNumber == 1) {
        // first page
        currentPage->addControl(EnumInputControl::create("Top skillset", searchObject.getTopSkillsetSetting()), m_buttonMenu);
        currentPage->addControl(EnumInputControl::create("Has skillset", searchObject.getHasSkillsetSetting()), m_buttonMenu);
        currentPage->addControl(EnumInputControl::create("Sort by", searchObject.getSortSetting()), m_buttonMenu);
        currentPage->addControl(RangeInputControl<int>::create("Tiers", searchObject.getRatingsSetting(), true), m_buttonMenu);
        currentPage->addControl(RangeInputControl<float>::create("Enjoyment rating", searchObject.getEnjoymentsSetting()), m_buttonMenu);
        currentPage->addControl(EnumInputControl::create("Sort direction", searchObject.getSortDirectionSetting()), m_buttonMenu);
        currentPage->addControl(CheckboxInputControl::create("No rated", searchObject.getExcludeRatedSetting(), "No unrated", searchObject.getExcludeUnratedSetting(), true), m_buttonMenu);
        currentPage->addControl(CheckboxInputControl::create("No rated enj.", searchObject.getExcludeRatedEnjoymentSetting(), "No unrated enj.", searchObject.getExcludeUnratedEnjoymentSetting(), true), m_buttonMenu);
        currentPage->addControl(CheckboxInputControl::create("Completed", searchObject.getCompletedSetting(), "Uncompleted", searchObject.getUncompletedSetting(), true), m_buttonMenu);
    } else if (pageNumber == 2) {
        currentPage->addControl(EnumInputControl::create("Difficulty", searchObject.getDifficultySetting()), m_buttonMenu);
        currentPage->addControl(EnumInputControl::create("Length", searchObject.getLengthSetting()), m_buttonMenu);
        currentPage->addControl(EnumInputControl::create("Two player type", searchObject.getTwoPlayerSetting()), m_buttonMenu);
        currentPage->addControl(RangeInputControl<int>::create("Submissions count", searchObject.getRatingsCountSetting(), true), m_buttonMenu);
        currentPage->addControl(RangeInputControl<int>::create("Enj. submissions count", searchObject.getEnjoymentsCountSetting(), true), m_buttonMenu);
        currentPage->addControl(RangeInputControl<float>::create("Rating deviation", searchObject.getDeviationsSetting()), m_buttonMenu);
        currentPage->addControl(TextInputControl::create("Creator", searchObject.getCreatorNameSetting()));
        currentPage->addControl(TextInputControl::create("Song", searchObject.getSongNameSetting()));
        currentPage->addControl(CheckboxInputControl::create("Is in pack", searchObject.getIsInPackSetting()), m_buttonMenu);

    } else if (pageNumber == 3) {
        currentPage->addControl(RangeInputControl<int>::create("Level ID", searchObject.getIdsRangeSetting(), true), m_buttonMenu);
    }
    updatePageNumberLabel();
}

void GDDLSearchLayerV2::updatePageNumberLabel() {
    pageNumberLabel->setString(pageNames[currentPageNumber].c_str());
    Utils::scaleLabelToWidth(pageNumberLabel, 110.0f);
}

void GDDLSearchLayerV2::onSearchClicked(CCObject* sender) {
    currentPage->saveSettings();
    searchObject.getLevelNameSetting()->setSettingValue(this->levelNameTextInput->getString());
    searchObject.createSearchQuery();
}

void GDDLSearchLayerV2::onResetClicked(CCObject* sender) {
    searchObject.resetToDefaults();
    levelNameTextInput->setString(searchObject.getLevelNameSetting()->getSettingValue());
    currentPage->reloadSettings();
}

void GDDLSearchLayerV2::onPageLeftClicked(CCObject* sender) {
    --currentPageNumber;
    if (currentPageNumber < 0) {
        currentPageNumber = 3;
    }
    reloadAfterSwitchingPage();
}

void GDDLSearchLayerV2::onPageRightClicked(CCObject* sender) {
    ++currentPageNumber;
    if (currentPageNumber > 3) {
        currentPageNumber = 0;
    }
    reloadAfterSwitchingPage();
}

void GDDLSearchLayerV2::onClose(CCObject* sender) {
    currentPage->saveSettings();
    searchObject.getLevelNameSetting()->setSettingValue(this->levelNameTextInput->getString());
    searchObject.saveSettings();
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

GDDLSearchLayerV2* GDDLSearchLayerV2::create() {
    const auto newLayer = new GDDLSearchLayerV2();
    // ReSharper disable once CppDFAConstantConditions
    if (newLayer != nullptr && newLayer->init()) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLSearchLayerV2::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}
