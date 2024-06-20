// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppDFAConstantParameter
#include "GDDLSearchLayer.h"

#include <Geode/utils/web.hpp>
#include <utility>

#include "RatingsManager.h"
#include "Utils.h"
#include "LevelBrowserLayer.cpp"

bool GDDLSearchLayer::init() {
    if (!FLAlertLayer::init(150))
        return false;

    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    // background
    const auto bg = CCScale9Sprite::create("GJ_square01.png", {0.0f, 0.0f, 80.0f, 80.0f});
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
    // close button
    const auto closeButtonSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    const auto closeButton =
            CCMenuItemSpriteExtra::create(closeButtonSprite, this, menu_selector(GDDLSearchLayer::onClose));
    m_buttonMenu->addChild(closeButton, 1);
    closeButton->setPosition({10.0f, 0.0f});
    // title
    const auto title = CCLabelBMFont::create("GDDL Search", "goldFont.fnt");
    m_buttonMenu->addChild(title, 1);
    title->setPosition({220.0f, -10.0f});
    title->setScale(0.7f);
    // info button
    const auto infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    const auto infoButton =
            CCMenuItemSpriteExtra::create(infoButtonSprite, this, menu_selector(GDDLSearchLayer::onInfo));
    m_buttonMenu->addChild(infoButton);
    m_buttonMenu->reorderChild(infoButton, 1);
    infoButton->setPosition({423.0f, -7.0f});
    // change interface button
    const auto changeInterfaceSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
    const auto changeInterfaceButton = CCMenuItemSpriteExtra::create(changeInterfaceSprite, this, menu_selector(GDDLSearchLayer::onSwapLayout));
    m_buttonMenu->addChild(changeInterfaceButton, 1);
    changeInterfaceButton->setPosition({popupSize.x-10.0f, -popupSize.y+17.0f});
    // interface
    normalMenu = CCMenu::create();
    normalMenu->setID("gddl-demon-new-search-menu"_spr);
    normalMenu->setContentSize({winSize.width, winSize.height});
    normalMenu->setPosition({64.5f - aspectRatioFixX, 295.0f - aspectRatioFixY});
    m_mainLayer->addChild(normalMenu, 1);
    simplifiedMenu = CCMenu::create();
    simplifiedMenu->setID("gddl-demon-old-search-menu"_spr);
    simplifiedMenu->setLayout(ColumnLayout::create()->setGap(5.0f)->setAxisReverse(true)->setAutoScale(true));
    m_mainLayer->addChild(simplifiedMenu, 1);
    showPage();
    loadValues();
    // prepare the search request fun
    searchListener.bind([this] (web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                const std::string response = res->string().unwrapOrDefault();
                if (response.empty()) {
                    FLAlertLayer::create("GDDL Search",
                    "Search failed - either you're disconnected from the internet or the server did something wrong...",
                        "OK")->show();
                } else {
                    appendFetchedResults(response);
                    auto [fst, snd] = getReadyRange(requestRequestedPage);
                    if (snd - fst < 10 && onlinePagesFetched < getOnlinePagesCount()) {
                        // recurse
                        const std::string anotherRequest = formSearchRequest();
                        auto req = web::WebRequest();
                        searchListener.setFilter(req.get(anotherRequest));
                    } else {
                        GJSearchObject *searchObject = makeASearchObjectFrom(fst, snd);
                        handleSearchObject(searchObject, searchCallbackObject, snd - fst);
                    }
                }
            } else if (e->isCancelled()) {
                FLAlertLayer::create("GDDL Search",
                  "Search failed - either you're disconnected from the internet or the server did something wrong...",
                  "OK")->show();
            }
        });
    return true;
}

void GDDLSearchLayer::loadPageFull() {
    // page - specific buttons
    // search button
    const auto searchButtonSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn06_001.png");
    searchButtonSprite->setScale(0.7f);
    searchButton =
            CCMenuItemSpriteExtra::create(searchButtonSprite, this, menu_selector(GDDLSearchLayer::onSearchClicked));
    normalMenu->addChild(searchButton, 1);
    searchButton->setPosition({310.0f, -10.0f});
    // reset button
    const auto resetButtonSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn07_001.png");
    resetButtonSprite->setScale(0.7f);
    resetButton =
            CCMenuItemSpriteExtra::create(resetButtonSprite, this, menu_selector(GDDLSearchLayer::onResetClicked));
    normalMenu->addChild(resetButton, 1);
    resetButton->setPosition({340.0f, -10.0f});
    // column 1
    // level name
    Utils::createLabel(normalMenu, "bigFont.fnt", "Name", 110.0f, {75.0f, -37.5f});
    Utils::createTextInputNode(normalMenu, nameTextfield, "bigFont.fnt", "", 32, {80.0f, 25.0f}, {60.0f, -62.5f});
    Utils::createCheckbox(normalMenu, nameExactMatchToggler, "Ex. match", 17.5f, 0.9f, {117.0f, -62.5f}, this,
                   menu_selector(GDDLSearchLayer::onToggleExactMatch));
    // creator
    Utils::createLabel(normalMenu, "bigFont.fnt", "Creator", 110.0f, {75.0f, -97.5f});
    Utils::createTextInputNode(normalMenu, creatorTextfield, "bigFont.fnt", "", 32, {110.0f, 25.0f}, {75.0f, -122.5f});
    // song name
    Utils::createLabel(normalMenu, "bigFont.fnt", "Song name", 110.0f, {75.0f, -157.5f});
    Utils::createTextInputNode(normalMenu, songTextfield, "bigFont.fnt", "", 32, {110.0f, 25.0f}, {75.0f, -182.5f});
    // in-game difficulty
    Utils::createLabel(normalMenu, "bigFont.fnt", "Difficulty", 110.0f, {75.0f, -217.5f});
    auto bg = Utils::createLabelForChoice(normalMenu, difficultyLabel, "bigFont.fnt", "Any", 110.0f, {75.0f, -242.5f},
                                   {110.0f, 25.0f});
    Utils::createLeftRightButtonsAround(bg, {13.0f, 19.0f}, this, menu_selector(GDDLSearchLayer::onInGameRatingLeft),
                                 menu_selector(GDDLSearchLayer::onInGameRatingRight));

    // column 2
    // rating range
    Utils::createLabel(normalMenu, "bigFont.fnt", "Tiers", 110.0f, {220.0f, -37.5f});
    Utils::createTextInputNode(normalMenu, tierLowTextfield, "bigFont.fnt", "", 2, {35.0f, 25.0f}, {182.5f, -62.5f});
    tierLowTextfield->setAllowedChars("1234567890");
    Utils::createTextInputNode(normalMenu, tierHighTextfield, "bigFont.fnt", "", 2, {35.0f, 25.0f}, {257.5f, -62.5f});
    tierHighTextfield->setAllowedChars("1234567890");
    Utils::createLeftRightButtonsAround(tierLowTextfield, {13.0f, 19.0f}, this, menu_selector(GDDLSearchLayer::onTierLowLeft),
                                 menu_selector(GDDLSearchLayer::onTierLowRight));
    Utils::createLeftRightButtonsAround(tierHighTextfield, {13.0f, 19.0f}, this, menu_selector(GDDLSearchLayer::onTierHighLeft),
                                 menu_selector(GDDLSearchLayer::onTierHighRight));
    Utils::createLabel(normalMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -62.5f});
    // enjoyment range
    Utils::createLabel(normalMenu, "bigFont.fnt", "Enjoyment rating", 110.0f, {220.0f, -97.5f});
    Utils::createTextInputNode(normalMenu, enjoymentLowTextfield, "bigFont.fnt", "", 5, {35.0f, 25.0f}, {182.5f, -122.5f});
    enjoymentLowTextfield->setAllowedChars("1234567890.");
    Utils::createTextInputNode(normalMenu, enjoymentHighTextfield, "bigFont.fnt", "", 5, {35.0f, 25.0f}, {257.5f, -122.5f});
    enjoymentHighTextfield->setAllowedChars("1234567890.");
    Utils::createLeftRightButtonsAround(enjoymentLowTextfield, {13.0f, 19.0f}, this,
                                 menu_selector(GDDLSearchLayer::onEnjoymentLowLeft),
                                 menu_selector(GDDLSearchLayer::onEnjoymentLowRight));
    Utils::createLeftRightButtonsAround(enjoymentHighTextfield, {13.0f, 19.0f}, this,
                                 menu_selector(GDDLSearchLayer::onEnjoymentHighLeft),
                                 menu_selector(GDDLSearchLayer::onEnjoymentHighRight));
    Utils::createLabel(normalMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -122.5f});
    // submissions count
    Utils::createLabel(normalMenu, "bigFont.fnt", "Submissions count", 110.0f, {220.0f, -157.5f});
    Utils::createTextInputNode(normalMenu, submissionsCountLowTextfield, "bigFont.fnt", "", 4, {35.0f, 25.0f},
                        {182.5f, -182.5f});
    submissionsCountLowTextfield->setAllowedChars("1234567890");
    Utils::createTextInputNode(normalMenu, submissionsCountHighTextfield, "bigFont.fnt", "", 4, {35.0f, 25.0f},
                        {257.5f, -182.5f});
    submissionsCountHighTextfield->setAllowedChars("1234567890");
    Utils::createLeftRightButtonsAround(submissionsCountLowTextfield, {13.0f, 19.0f}, this,
                                 menu_selector(GDDLSearchLayer::onSubmissionCountLowLeft),
                                 menu_selector(GDDLSearchLayer::onSubmissionCountLowRight));
    Utils::createLeftRightButtonsAround(submissionsCountHighTextfield, {13.0f, 19.0f}, this,
                                 menu_selector(GDDLSearchLayer::onSubmissionCountHighLeft),
                                 menu_selector(GDDLSearchLayer::onSubmissionCountHighRight));
    Utils::createLabel(normalMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -182.5f});
    // enjoyment submissions count
    Utils::createLabel(normalMenu, "bigFont.fnt", "Enj. submissions count", 110.0f, {220.0f, -217.5f});
    Utils::createTextInputNode(normalMenu, enjSubmissionsCountLowTextfield, "bigFont.fnt", "", 4, {35.0f, 25.0f},
                        {182.5f, -242.5f});
    enjSubmissionsCountLowTextfield->setAllowedChars("1234567890");
    Utils::createTextInputNode(normalMenu, enjSubmissionsCountHighTextfield, "bigFont.fnt", "", 4, {35.0f, 25.0f},
                        {257.5f, -242.5f});
    enjSubmissionsCountHighTextfield->setAllowedChars("1234567890");
    Utils::createLeftRightButtonsAround(enjSubmissionsCountLowTextfield, {13.0f, 19.0f}, this,
                                 menu_selector(GDDLSearchLayer::onEnjSubmissionCountLowLeft),
                                 menu_selector(GDDLSearchLayer::onEnjSubmissionCountLowRight));
    Utils::createLeftRightButtonsAround(enjSubmissionsCountHighTextfield, {13.0f, 19.0f}, this,
                                 menu_selector(GDDLSearchLayer::onEnjSubmissionCountHighLeft),
                                 menu_selector(GDDLSearchLayer::onEnjSubmissionCountHighRight));
    Utils::createLabel(normalMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -242.5f});

    // column 3
    // tiers checkboxes
    Utils::createCheckbox(normalMenu, noUnratedToggler, "No unrated", 17.5f, 0.9f, {325.0f, -62.5f}, this,
                   menu_selector(GDDLSearchLayer::onToggleNoUnrated));
    Utils::createCheckbox(normalMenu, noRatedToggler, "No rated", -19.5f, 0.9f, {365.0f, -62.5f}, this,
                   menu_selector(GDDLSearchLayer::onToggleNoRated));
    Utils::createCheckbox(normalMenu, completedToggler, "Completed", 17.5f, 0.9f, {405.0f, -62.5f}, this,
                   menu_selector(GDDLSearchLayer::onToggleCompleted));
    // enjoyment rating checkboxes
    Utils::createCheckbox(normalMenu, noUnratedEnjToggler, "No unr. enj.", 17.5f, 0.9f, {325.0f, -122.5f}, this,
                   menu_selector(GDDLSearchLayer::onToggleNoUnratedEnj));
    Utils::createCheckbox(normalMenu, noRatedEnjToggler, "No rated enj.", -19.5f, 0.9f, {365.0f, -122.5f}, this,
                   menu_selector(GDDLSearchLayer::onToggleNoRatedEnj));
    Utils::createCheckbox(normalMenu, uncompletedToggler, "Uncompleted", 17.5f, 0.9f, {405.0f, -122.5f}, this,
                   menu_selector(GDDLSearchLayer::onToggleUncompleted));
    // sort by
    Utils::createLabel(normalMenu, "bigFont.fnt", "Sort by", 110.0f, {365.0f, -157.5f});
    bg = Utils::createLabelForChoice(normalMenu, sortByLabel, "bigFont.fnt", "ID", 110.0f, {365.0f, -182.5f}, {110.0f, 25.0f});
    Utils::createLeftRightButtonsAround(bg, {13.0f, 19.0f}, this, menu_selector(GDDLSearchLayer::onSortByLeft),
                                 menu_selector(GDDLSearchLayer::onSortByRight));
    // sort direction
    Utils::createLabel(normalMenu, "bigFont.fnt", "Sort direction", 110.0f, {365.0f, -217.5f});
    bg = Utils::createLabelForChoice(normalMenu, sortDirectionLabel, "bigFont.fnt", "Ascending", 110.0f, {365.0f, -242.5f},
                              {110.0f, 25.0f});
    Utils::createLeftRightButtonsAround(bg, {13.0f, 19.0f}, this, menu_selector(GDDLSearchLayer::onSortDirectionLeft),
                                 menu_selector(GDDLSearchLayer::onSortDirectionRight));
    normalLoaded = true;
}

// because some people actually prefer the old ui
void GDDLSearchLayer::loadPageSimple() {
    // let's see what happens if I copypaste the old ui lmao >:)
    // tier buttons
    for (int row = 0; row < 5; row++) {
        const auto rowNode = CCMenu::create();
        rowNode->setLayout(RowLayout::create()->setGap(5.0f));
        rowNode->setContentSize({popupSize.x-10.0f, 20.0f});
        for (int column = 0; column < 7; column++) {
            const auto tierNode = createTierNode(row+1+column*5);
            rowNode->addChild(tierNode);
            tierButtons.push_back(tierNode);
        }
        rowNode->updateLayout();
        simplifiedMenu->addChild(rowNode);
        simplifiedMenu->reorderChild(rowNode, row+1);
    }
    // togglers - container
    const auto rowNode = CCMenu::create();
    rowNode->setLayout(RowLayout::create()->setGap(5.0f));
    rowNode->setContentSize({popupSize.x-10.0f, 35.0f});
    // unknown tier
    const auto unknownButton = createTierNode(-1);
    rowNode->addChild(unknownButton);
    tierButtons.push_back(unknownButton);
    // togglers - checkboxes
    const std::string ids[2] = {"completed", "uncompleted"};
    const std::string labels[2] = {"Completed", "Uncompleted"};
    const auto togglerNode = createCheckboxNode("completed", "Completed", completedTogglerSimple, menu_selector(GDDLSearchLayer::onToggleCompleted));
    rowNode->addChild(togglerNode);
    rowNode->reorderChild(togglerNode, 0);
    const auto togglerNode2 = createCheckboxNode("uncompleted", "Uncompleted", uncompletedTogglerSimple, menu_selector(GDDLSearchLayer::onToggleUncompleted));
    rowNode->addChild(togglerNode2);
    rowNode->reorderChild(togglerNode2, 1);
    rowNode->updateLayout();
    simplifiedMenu->addChild(rowNode);
    simplifiedMenu->reorderChild(rowNode, 9);
    simplifiedMenu->updateLayout();
    simplifiedLoaded = true;
}

void GDDLSearchLayer::showPage() {
    if(!simplified) { // hide all simplified and show all complex ones
        simplifiedMenu->setVisible(false);
        if(!normalLoaded) {
            loadPageFull();
        }
        normalMenu->setVisible(true);
        m_mainLayer->reorderChild(simplifiedMenu, 0);
        m_mainLayer->reorderChild(normalMenu, 1);
    } else { // the other way lmao
        clickOffTextfields(); // typing into invisible textfields crash prevention
        normalMenu->setVisible(false);
        if(!simplifiedLoaded) {
            loadPageSimple();
        }
        simplifiedMenu->setVisible(true);
        m_mainLayer->reorderChild(normalMenu, 0);
        m_mainLayer->reorderChild(simplifiedMenu, 1);
    }
    cocos::handleTouchPriority(this);
}

void GDDLSearchLayer::loadValues() {
    if(!simplified) {
        page = 1;
        nameTextfield->setString(name.c_str());
        Utils::setNumberWithDefZeroTextfield(lowTier, tierLowTextfield);
        Utils::setNumberWithDefZeroTextfield(highTier, tierHighTextfield);
        setDifficultyLabel();
        creatorTextfield->setString(creator.c_str());
        songTextfield->setString(song.c_str());
        nameExactMatchToggler->toggle(exactName);
        noUnratedToggler->toggle(removeUnrated);
        noUnratedEnjToggler->toggle(removeUnratedEnj);
        noRatedToggler->toggle(removeRated);
        noRatedEnjToggler->toggle(removeRatedEnj);
        completedToggler->toggle(completed);
        uncompletedToggler->toggle(uncompleted);
        Utils::setNumberWithDefZeroTextfield(subLowCount, submissionsCountLowTextfield);
        Utils::setNumberWithDefZeroTextfield(subHighCount, submissionsCountHighTextfield);
        Utils::setNumberWithDefZeroTextfield(enjLowCount, enjSubmissionsCountLowTextfield);
        Utils::setNumberWithDefZeroTextfield(enjHighCount, enjSubmissionsCountHighTextfield);
        setNumberFloatTextfield(enjLow, enjoymentLowTextfield);
        setNumberFloatTextfield(enjHigh, enjoymentHighTextfield);
        setSortDirectionLabel();
        setSortByLabel();
    } else {
        completedTogglerSimple->toggle(completed);
        uncompletedTogglerSimple->toggle(uncompleted);
    }
}

void GDDLSearchLayer::saveValues() {
    // fields with a set of choices don't have to be saved
    if(!simplified) {
        name = nameTextfield->getString();
        lowTier = Utils::getNumberTextfieldValue(tierLowTextfield);
        highTier = Utils::getNumberTextfieldValue(tierHighTextfield);
        creator = creatorTextfield->getString();
        song = songTextfield->getString();
        exactName = nameExactMatchToggler->isToggled();
        removeUnrated = noUnratedToggler->isToggled();
        removeUnratedEnj = noUnratedEnjToggler->isToggled();
        removeRated = noRatedToggler->isToggled();
        removeRatedEnj = noRatedEnjToggler->isToggled();
        completed = completedToggler->isToggled();
        uncompleted = uncompletedToggler->isToggled();
        subLowCount = Utils::getNumberTextfieldValue(submissionsCountLowTextfield);
        subHighCount = Utils::getNumberTextfieldValue(submissionsCountHighTextfield);
        enjLowCount = Utils::getNumberTextfieldValue(enjSubmissionsCountLowTextfield);
        enjHighCount = Utils::getNumberTextfieldValue(enjSubmissionsCountHighTextfield);
        enjLow = getFloatTextfieldValue(enjoymentLowTextfield, 0.0f);
        enjHigh = getFloatTextfieldValue(enjoymentHighTextfield, 10.0f);
    } else {
        completed = completedTogglerSimple->isToggled();
        uncompleted = uncompletedTogglerSimple->isToggled();
    }
}

// ReSharper disable once CppDFAUnreachableFunctionCall
void GDDLSearchLayer::resetValues() {
    page = 1;
    nameTextfield->setString("");
    Utils::setNumberWithDefZeroTextfield(0, tierLowTextfield);
    Utils::setNumberWithDefZeroTextfield(0, tierHighTextfield);
    difficulty = 5;
    setDifficultyLabel();
    creatorTextfield->setString("");
    songTextfield->setString("");
    nameExactMatchToggler->toggle(false);
    noUnratedToggler->toggle(false);
    noUnratedEnjToggler->toggle(false);
    noRatedToggler->toggle(false);
    noRatedEnjToggler->toggle(false);
    completedToggler->toggle(true);
    uncompletedToggler->toggle(true);
    Utils::setNumberWithDefZeroTextfield(0, submissionsCountLowTextfield);
    Utils::setNumberWithDefZeroTextfield(0, submissionsCountHighTextfield);
    Utils::setNumberWithDefZeroTextfield(0, enjSubmissionsCountLowTextfield);
    Utils::setNumberWithDefZeroTextfield(0, enjSubmissionsCountHighTextfield);
    setNumberFloatTextfield(0.0f, enjoymentLowTextfield);
    setNumberFloatTextfield(highestEnjoyment, enjoymentHighTextfield);
    sortDirectionIndex = 0;
    setSortDirectionLabel();
    sortOptionIndex = 0;
    setSortByLabel();
}

// For GDDL Demon Split (it becomes more accurate if you go though this instead of a local list)
void GDDLSearchLayer::cacheValues() {
    savedLowTier = lowTier;
    savedHighTier = highTier;
    savedCompleted = completed;
    savedUncompleted = uncompleted;
    // and reset all other ones as well
    savedName = name;
    name = "";
    savedDifficulty = difficulty;
    difficulty = 5;
    savedCreator = creator;
    creator = "";
    savedSong = song;
    song = "";
    savedExactName = exactName;
    exactName = false;
    savedRemoveRated = removeRated;
    removeRated = false;
    savedRemoveUnrated = removeUnrated;
    removeUnrated = false;
    savedRemoveRatedEnj = removeRatedEnj;
    removeRatedEnj = false;
    savedRemoveUnratedEnj = removeUnratedEnj;
    removeUnratedEnj = false;
    savedSubLowCount = subLowCount;
    subLowCount = 0;
    savedSubHighCount = subHighCount;
    subHighCount = 0;
    savedEnjLowCount = enjLowCount;
    enjLowCount = 0;
    savedEnjHighCount = enjHighCount;
    enjHighCount = 0;
    savedEnjLow = enjLow;
    enjLow = 0;
    savedEnjHigh = enjHigh;
    enjHigh = highestEnjoyment;
    savedSortOptionIndex = sortOptionIndex;
    sortOptionIndex = 0;
    savedSortDirectionIndex = sortDirectionIndex;
    sortDirectionIndex = 0;
}

void GDDLSearchLayer::restoreValues() {
    lowTier = savedLowTier;
    highTier = savedHighTier;
    completed = savedCompleted;
    uncompleted = savedUncompleted;
    name = savedName;
    difficulty = savedDifficulty;
    creator = savedCreator;
    song = savedSong;
    exactName = savedExactName;
    removeRated = savedRemoveRated;
    removeUnrated = savedRemoveUnrated;
    removeRatedEnj = savedRemoveRatedEnj;
    removeUnratedEnj = savedRemoveUnratedEnj;
    subLowCount = savedSubLowCount;
    subHighCount = savedSubHighCount;
    enjLowCount = savedEnjLowCount;
    enjHighCount = savedEnjHighCount;
    enjLow = savedEnjLow;
    enjHigh = savedEnjHigh;
    sortOptionIndex = savedSortOptionIndex;
    sortDirectionIndex = savedSortDirectionIndex;
}

void GDDLSearchLayer::onClose(CCObject *sender) {
    saveValues();
    saveSettings();
    simplifiedLoaded = false;
    normalLoaded = false;
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLSearchLayer::keyBackClicked() {
    saveValues();
    FLAlertLayer::keyBackClicked(); // calls onClose I think
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void GDDLSearchLayer::onInfo(CCObject *sender) { // NOLINT(*-convert-member-functions-to-static)
    std::string infoContent =
            "<co>Name - Ex. match</c> - will <cy>only</c> search for levels with <cy>exactly</c> matching "
            "names\n<co>Difficulty</c> - <cy>in-game</c> <cr>demon</c> difficulty\n<co>Enj./Submissions count</c> - "
            "the amount of <cj>enjoyment</c>/<cp>difficulty tier</c> submissions for a level\n<co>No unrated/No rated "
            "(Enj)</c> - <cy>only</c> search for levels <cg>with</c>(<cr>out</c>) a <cp>tier</c>/<cj>enjoyment</c> "
            "rating\n<cb>All</c> tier and submission ranges are <cy>inclusive</c> from <cy>both</c> sides";
    if (simplified) {
        infoContent = "Select the <cg>completed</c>/<cr>uncompleted</c> checkboxes and click a <co>button</c> to search for a corresponding <cy>tier</c>, click the <cp>?</c> button to search for levels that are <cb>not yet rated</c>";
    }
    FLAlertLayer::create("GDDL Search Help", infoContent, "OK")->show();
}

std::string GDDLSearchLayer::urlEncodeString(std::string toEncode) {
    if (toEncode.find(' ') != std::string::npos) { // that's the only character that might need it
        std::string encoded;
        for (const auto character: toEncode) {
            if (character == ' ') {
                encoded += "%20";
            } else {
                encoded += character;
            }
        }
        return encoded;
    }
    return toEncode;
}

std::string GDDLSearchLayer::addStringToRequest(const std::string &paramName, const std::string &value) {
    if (value.empty()) {
        return "";
    }
    return "&" + paramName + "=" + urlEncodeString(value);
}

std::string GDDLSearchLayer::addBoolToRequest(const std::string &paramName, const bool value) {
    std::string strValue = "true";
    if (!value) {
        strValue = "false";
    }
    return "&" + paramName + "=" + strValue;
}

std::string GDDLSearchLayer::formSearchRequest() {
    std::string request = searchEndpoint;
    request += "?page=" + std::to_string(onlinePagesFetched + 1) + "&chunk=50";
    request += addStringToRequest("name", name);
    request += addValueToRequest("lowTier", lowTier, 0);
    request += addValueToRequest("highTier", highTier, 0);
    request += addValueToRequest("difficulty", difficulty+1, 6); // API 1.9.0 - diffs 1-5
    request += addStringToRequest("creator", creator);
    request += addStringToRequest("song", song);
    request += addBoolToRequest("exactName", exactName);
    request += addBoolToRequest("removeUnrated", removeUnrated);
    request += addBoolToRequest("removeUnratedEnj", removeUnratedEnj);
    request += addBoolToRequest("removeRated", removeRated);
    request += addBoolToRequest("removeRatedEnj", removeRatedEnj);
    request += addValueToRequest("subLowCount", subLowCount, 0);
    request += addValueToRequest("subHighCount", subHighCount, 0);
    request += addValueToRequest("enjLowCount", enjLowCount, 0);
    request += addValueToRequest("enjHighCount", enjHighCount, 0);
    request += addValueToRequest("enjLow", enjLow, 0.0f);
    request += addValueToRequest("enjHigh", enjHigh, highestEnjoyment);
    request += addStringToRequest("sort", sort[sortOptionIndex]);
    request += addStringToRequest("sortDirection", sortDirection[sortDirectionIndex]);
    // log::debug("Search request: {}", request);
    return request;
}

/**
 * example response:
{
  "total": 1,
  "limit": 16,
  "page": 1,
  "levels": [
    {
      "LevelID": 30380566,
      "Name": "Bausha Vortex",
      "Rating": 28,
      "Enjoyment": 8.285714285714286,
      "Creator": "Pennutoh",
      "Song": "~NK~ Fracture",
      "Difficulty": "Extreme",
      "InPack": 1
    }
  ]
}
 */

std::vector<int> GDDLSearchLayer::parseResponse(const std::string& response) {
    std::vector<int> results;
    try {
        matjson::Value responseJson = matjson::parse(response);
        const int total = responseJson["total"].as_int();
        totalOnlineResults = std::max(totalOnlineResults, total); // so it never grabs 0 if a bad request is made
        matjson::Value levelList = responseJson["levels"];
        for (auto element: levelList.as_array()) {
            const int levelID = element["LevelID"].as_int();
            if (levelID > 3) { // to avoid official demons
                results.push_back(element["LevelID"].as_int());
                if(!element["Rating"].is_null()) {
                    const float rating = element["Rating"].as_double();
                    RatingsManager::updateCacheFromSearch(levelID, rating);
                }
            }
        }
    } catch (std::runtime_error &error) {
        // well nothing really can be done here
    }
    return results;
}

std::vector<int> GDDLSearchLayer::filterResults(std::vector<int> ids, const LevelCompleteness completionStatus) {
    std::set<int> setOfIds = Utils::copyVectorToSet(std::move(ids));
    std::vector<int> filteredList;
    if (completionStatus != ANY) {
        std::set<int> allCompleted;
        GameLevelManager *levelManager = GameLevelManager::sharedState();
        const cocos2d::CCArray *completedLevels = levelManager->getCompletedLevels(false);
        CCObject *obj;
        CCARRAY_FOREACH(completedLevels, obj) {
            const auto level = dynamic_cast<GJGameLevel *>(obj);
            // ReSharper disable once CppTooWideScopeInitStatement
            const bool levelCompleted = level->m_normalPercent == 100;
            if (setOfIds.contains(level->m_levelID) && levelCompleted) {
                allCompleted.insert(level->m_levelID);
            }
        }
        if (completionStatus == COMPLETED) {
            filteredList = Utils::copySetToVector(allCompleted);
        } else { // searchType == UNCOMPLETED
            for (auto level: allCompleted) {
                setOfIds.erase(level);
            }
            filteredList = Utils::copySetToVector(setOfIds);
        }
    } else {
        filteredList = Utils::copySetToVector(setOfIds);
    }
    return filteredList;
}

int GDDLSearchLayer::getMaxPotentialPages() {
    const int correction = totalOnlineResults % 10 == 0 ? 0 : 1;
    return totalOnlineResults / 10 + correction;
}

int GDDLSearchLayer::getOnlinePagesCount() {
    const int correction = totalOnlineResults % 50 == 0 ? 0 : 1;
    return totalOnlineResults / 50 + correction;
}

GJSearchObject *GDDLSearchLayer::makeASearchObjectFrom(const int firstIndex, const int lastIndex) {
    std::string requestString;
    for (int i = firstIndex; i < lastIndex; i++) {
        const int id = cachedResults[i];
        requestString += std::to_string(cachedResults[i]) + ',';
    }
    if (!requestString.empty()) {
        requestString.pop_back();
    }
    requestString += "&gameVersion=22";
    return GJSearchObject::create(SearchType::Type19, requestString);
}

void GDDLSearchLayer::appendFetchedResults(const std::string& response) {
    const std::vector<int> parsedResponse = parseResponse(response);
    if (completed == uncompleted) {
        completeness = ANY;
    } else {
        if (completed) {
            completeness = COMPLETED;
        } else {
            completeness = UNCOMPLETED;
        }
    }
    const std::vector<int> filteredResponse = filterResults(parsedResponse, completeness);
    for (auto element: filteredResponse) {
        cachedResults.push_back(element);
    }
    onlinePagesFetched++;
}

std::pair<int, int> GDDLSearchLayer::getReadyRange(const int requestedPage) {
    const int firstIndex = (requestedPage - 1) * 10;
    const int lastIndex = std::min(firstIndex + 10, static_cast<int>(cachedResults.size())); // last index + 1
    return {firstIndex, lastIndex};
}

void GDDLSearchLayer::handleSearchObject(GJSearchObject *searchObject, GDDLBrowserLayer* callbackObject,
                                         const int resultsCount) {
    if(callbackObject != nullptr) { // search continues
        callbackObject->handleSearchObject(searchObject, resultsCount);
    } else { // new search
        const auto listLayer = LevelBrowserLayer::create(searchObject);
        const auto listLayerScene = CCScene::create();
        listLayerScene->addChild(listLayer);
        auto transition = CCTransitionFade::create(0.5, listLayerScene);
        CCDirector::sharedDirector()->pushScene(transition);
    }
}




// ReSharper disable once CppDFAUnreachableFunctionCall NOT TRUE
float GDDLSearchLayer::calculateNewFloat(const float currentValue, const bool increase, const float lowerbound,
                                         const float upperbound) {
    float newValue = currentValue + (increase ? 1.0f : -1.0f);
    if (std::abs(currentValue - std::floor(currentValue)) >= 0.01f) {
        newValue = increase ? std::floor(currentValue) + 1 : std::floor(currentValue);
    }
    if (newValue < lowerbound)
        newValue = upperbound;
    if (newValue > upperbound)
        newValue = lowerbound;
    return newValue;
}

CCMenuItemSpriteExtra *GDDLSearchLayer::createTierNode(const int tier) {
    // tier sprite
    const std::string tierString = tier != -1 ? std::to_string(tier) : "unrated";
    const std::string tierSpriteName = "tier_" + tierString + ".png";
    const auto tierSprite = CCSprite::create(Mod::get()->expandSpriteName(tierSpriteName.c_str()).data());
    tierSprite->setScale(0.24f);
    tierSprite->setContentSize({30.0f, 30.0f});
    const auto tierButton = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(GDDLSearchLayer::onTierSearch));
    tierButton->setContentSize({30.0f, 30.0f});
    tierButton->setID("button-tier-" + std::to_string(tier));
    return tierButton;
}

CCMenu *GDDLSearchLayer::createCheckboxNode(const std::string &idSuffix, const std::string &name,
                                            CCMenuItemToggler *&toggler, const SEL_MenuHandler callback) {
    const auto menu = CCMenu::create();
    menu->setLayout(RowLayout::create()->setGap(3.0f)->setAutoScale(true));
    // checkbox
    toggler = CCMenuItemToggler::createWithStandardSprites(this, callback, 0.8f);
    toggler->setID("toggler-" + idSuffix);
    menu->addChild(toggler);
    menu->reorderChild(toggler, 0);
    // label
    const auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setContentSize({110.0f, 35.0f});
    menu->addChild(label);
    menu->reorderChild(label, 1);
    menu->setContentSize({150.0f, 35.0f});
    menu->updateLayout();
    label->setScale(110.0f / label->getContentSize().width);
    return menu;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void GDDLSearchLayer::onToggleExactMatch(CCObject *sender) { // NOLINT(*-convert-member-functions-to-static)
    exactName = !dynamic_cast<CCMenuItemToggler *>(sender)->isOn();
}

void GDDLSearchLayer::onInGameRatingLeft(CCObject *sender) {
    difficulty--;
    if (difficulty < 0)
        difficulty = demonDifficulties.size() - 1; // NOLINT(*-narrowing-conversions)
    setDifficultyLabel();
}

void GDDLSearchLayer::onInGameRatingRight(CCObject *sender) {
    difficulty++;
    if (difficulty > demonDifficulties.size() - 1)
        difficulty = 0;
    setDifficultyLabel();
}

void GDDLSearchLayer::onTierLowLeft(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(tierLowTextfield) - 1;
    if (newValue < 0)
        newValue = highestTier;
    Utils::setNumberWithDefZeroTextfield(newValue, tierLowTextfield);
}
void GDDLSearchLayer::onTierLowRight(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(tierLowTextfield) + 1;
    if (newValue > highestTier)
        newValue = 0;
    Utils::setNumberWithDefZeroTextfield(newValue, tierLowTextfield);
}

void GDDLSearchLayer::onTierHighLeft(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(tierHighTextfield) - 1;
    if (newValue < 0)
        newValue = highestTier;
    Utils::setNumberWithDefZeroTextfield(newValue, tierHighTextfield);
}

void GDDLSearchLayer::onTierHighRight(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(tierHighTextfield) + 1;
    if (newValue > highestTier)
        newValue = 0;
    Utils::setNumberWithDefZeroTextfield(newValue, tierHighTextfield);
}

void GDDLSearchLayer::onEnjoymentLowLeft(CCObject *sender) {
    const float currentValue = getFloatTextfieldValue(enjoymentLowTextfield, 0.0f);
    const float newValue = calculateNewFloat(currentValue, false, 0.0f, highestEnjoyment);
    setNumberFloatTextfield(newValue, enjoymentLowTextfield);
}

void GDDLSearchLayer::onEnjoymentLowRight(CCObject *sender) {
    const float currentValue = getFloatTextfieldValue(enjoymentLowTextfield, 0.0f);
    const float newValue = calculateNewFloat(currentValue, true, 0.0f, highestEnjoyment);
    setNumberFloatTextfield(newValue, enjoymentLowTextfield);
}

void GDDLSearchLayer::onEnjoymentHighLeft(CCObject *sender) {
    const float currentValue = getFloatTextfieldValue(enjoymentHighTextfield, 10.0f);
    const float newValue = calculateNewFloat(currentValue, false, 0.0f, highestEnjoyment);
    setNumberFloatTextfield(newValue, enjoymentHighTextfield);
}

void GDDLSearchLayer::onEnjoymentHighRight(CCObject *sender) {
    const float currentValue = getFloatTextfieldValue(enjoymentHighTextfield, 10.0f);
    const float newValue = calculateNewFloat(currentValue, true, 0.0f, highestEnjoyment);
    setNumberFloatTextfield(newValue, enjoymentHighTextfield);
}

void GDDLSearchLayer::onSubmissionCountLowLeft(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(submissionsCountLowTextfield) - 1;
    if (newValue < 0)
        newValue = 0;
    Utils::setNumberWithDefZeroTextfield(newValue, submissionsCountLowTextfield);
}

void GDDLSearchLayer::onSubmissionCountLowRight(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(submissionsCountLowTextfield) + 1;
    if (newValue > maxSubmissions)
        newValue = maxSubmissions;
    Utils::setNumberWithDefZeroTextfield(newValue, submissionsCountLowTextfield);
}

void GDDLSearchLayer::onSubmissionCountHighLeft(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(submissionsCountHighTextfield) - 1;
    if (newValue < 0)
        newValue = 0;
    Utils::setNumberWithDefZeroTextfield(newValue, submissionsCountHighTextfield);
}

void GDDLSearchLayer::onSubmissionCountHighRight(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(submissionsCountHighTextfield) + 1;
    if (newValue > maxSubmissions)
        newValue = maxSubmissions;
    Utils::setNumberWithDefZeroTextfield(newValue, submissionsCountHighTextfield);
}

void GDDLSearchLayer::onEnjSubmissionCountLowLeft(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(enjSubmissionsCountLowTextfield) - 1;
    if (newValue < 0)
        newValue = 0;
    Utils::setNumberWithDefZeroTextfield(newValue, enjSubmissionsCountLowTextfield);
}

void GDDLSearchLayer::onEnjSubmissionCountLowRight(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(enjSubmissionsCountLowTextfield) + 1;
    if (newValue > maxSubmissions)
        newValue = maxSubmissions;
    Utils::setNumberWithDefZeroTextfield(newValue, enjSubmissionsCountLowTextfield);
}

void GDDLSearchLayer::onEnjSubmissionCountHighLeft(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(enjSubmissionsCountHighTextfield) - 1;
    if (newValue < 0)
        newValue = 0;
    Utils::setNumberWithDefZeroTextfield(newValue, enjSubmissionsCountHighTextfield);
}

void GDDLSearchLayer::onEnjSubmissionCountHighRight(CCObject *sender) {
    int newValue = Utils::getNumberTextfieldValue(enjSubmissionsCountHighTextfield) + 1;
    if (newValue > maxSubmissions)
        newValue = maxSubmissions;
    Utils::setNumberWithDefZeroTextfield(newValue, enjSubmissionsCountHighTextfield);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void GDDLSearchLayer::onToggleNoUnrated(CCObject *sender) {
    if (!noUnratedToggler->isOn()) {
        noRatedToggler->toggle(false);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void GDDLSearchLayer::onToggleNoRated(CCObject *sender) {
    if (!noRatedToggler->isOn()) {
        noUnratedToggler->toggle(false);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void GDDLSearchLayer::onToggleNoUnratedEnj(CCObject *sender) {
    if (!noUnratedEnjToggler->isOn()) {
        noRatedEnjToggler->toggle(false);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void GDDLSearchLayer::onToggleNoRatedEnj(CCObject *sender) {
    if (!noRatedEnjToggler->isOn()) {
        noUnratedEnjToggler->toggle(false);
    }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void GDDLSearchLayer::onToggleCompleted(CCObject *sender) {
    // do nothing
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void GDDLSearchLayer::onToggleUncompleted(CCObject *sender) {
     // do nothing
}

void GDDLSearchLayer::onSortByLeft(CCObject *sender) {
    sortOptionIndex--;
    if (sortOptionIndex < 0)
        sortOptionIndex = sort.size() - 1; // NOLINT(*-narrowing-conversions)
    setSortByLabel();
}

void GDDLSearchLayer::onSortByRight(CCObject *sender) {
    sortOptionIndex++;
    if (sortOptionIndex > sort.size() - 1)
        sortOptionIndex = 0;
    setSortByLabel();
}

void GDDLSearchLayer::onSortDirectionLeft(CCObject *sender) {
    sortDirectionIndex--;
    if (sortDirectionIndex < 0)
        sortDirectionIndex = sortDirection.size() - 1; // NOLINT(*-narrowing-conversions)
    setSortDirectionLabel();
}

void GDDLSearchLayer::onSortDirectionRight(CCObject *sender) {
    sortDirectionIndex++;
    if (sortDirectionIndex > sortDirection.size() - 1)
        sortDirectionIndex = 0;
    setSortDirectionLabel();
}

void GDDLSearchLayer::onSearchClicked(CCObject *sender) {
    if (searching) return; // super good multithreaded code
    saveValues();
    totalOnlineResults = 0;
    cachedResults.clear();
    onlinePagesFetched = 0;
    searching = true;
    requestSearchPage(1, nullptr);
}

void GDDLSearchLayer::onResetClicked(CCObject *sender) { resetValues(); }

void GDDLSearchLayer::onSwapLayout(CCObject *sender) {
    saveValues();
    simplified = !simplified;
    showPage();
    loadValues();
}

void GDDLSearchLayer::onTierSearch(CCObject *sender) {
    if (searching) return; // super good multithreaded code
    // save values before replacing them
    saveValues();
    cacheValues();
    // and then
    auto *senderNode = dynamic_cast<CCNode *>(sender);
    const std::string tierStr = senderNode->getID();
    const int tierNumber = std::stoi(tierStr.substr(12, tierStr.size()-10)); // always valid
    if(tierNumber != -1) {
        lowTier = tierNumber;
        highTier = tierNumber;
    } else {
        removeRated = true;
        lowTier = 0;
        highTier = 0;
    }
    completed = completedTogglerSimple->isToggled();
    uncompleted = uncompletedTogglerSimple->isToggled();
    totalOnlineResults = 0;
    cachedResults.clear();
    onlinePagesFetched = 0;
    searching = true;
    requestSearchPage(1, nullptr);
}

void GDDLSearchLayer::setNumberFloatTextfield(const float value, CCTextInputNode *&textfield) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << value;
    textfield->setString(ss.str().c_str());
}

void GDDLSearchLayer::setDifficultyLabel() {
    std::string newValue = demonDifficulties[difficulty];
    if (difficulty != 5)
        newValue += " Demon";
    difficultyLabel->setString(newValue.c_str());
    Utils::scaleLabelToWidth(difficultyLabel, 110.0f);
}

void GDDLSearchLayer::setSortByLabel() {
    sortByLabel->setString(sortBy[sortOptionIndex].c_str());
    Utils::scaleLabelToWidth(sortByLabel, 110.0f);
}

void GDDLSearchLayer::setSortDirectionLabel() {
    sortDirectionLabel->setString(orderDirection[sortDirectionIndex].c_str());
    Utils::scaleLabelToWidth(sortDirectionLabel, 110.0f);
}

float GDDLSearchLayer::getFloatTextfieldValue(CCTextInputNode *&textfield, float defaultValue) {
    if (textfield->getString().empty())
        return 0;
    float returnValue = defaultValue;
    auto returnValueResult = numFromString<float>(textfield->getString());
    if (returnValueResult.isOk()) {
        returnValue = returnValueResult.value();
    }
    return returnValue;
}

void GDDLSearchLayer::onEnter() {
    FLAlertLayer::onEnter();
    cocos::handleTouchPriority(this);
    stopSearch();
    restoreValuesAfterSplit(); // scene switching won't come back to the init() reset in creatorlayer
}

GDDLSearchLayer *GDDLSearchLayer::create() {
    const auto newLayer = new GDDLSearchLayer();
    // ReSharper disable once CppDFAConstantConditions
    if (newLayer != nullptr && newLayer->init()) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLSearchLayer::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}

void GDDLSearchLayer::loadSettings() {
    // happens only on startup
    if (!firstLoad) return;
    firstLoad = false;
    name = Mod::get()->getSavedValue<std::string>("search-name", "");
    lowTier = Mod::get()->getSavedValue<int>("search-lowTier", 0);
    highTier = Mod::get()->getSavedValue<int>("search-highTier", 0);
    difficulty = Mod::get()->getSavedValue<int>("search-difficulty", 5);
    creator = Mod::get()->getSavedValue<std::string>("search-creator", "");
    song = Mod::get()->getSavedValue<std::string>("search-song", "");
    exactName = Mod::get()->getSavedValue<bool>("search-exactName", false);
    removeUnrated = Mod::get()->getSavedValue<bool>("search-removeUnrated", false);
    removeUnratedEnj = Mod::get()->getSavedValue<bool>("search-removeUnratedEnj", false);
    removeRated = Mod::get()->getSavedValue<bool>("search-removeRated", false);
    removeRatedEnj = Mod::get()->getSavedValue<bool>("search-removeRatedEnj", false);
    subLowCount = Mod::get()->getSavedValue<int>("search-subLowCount", 0);
    subHighCount = Mod::get()->getSavedValue<int>("search-subHighCount", 0);
    enjLowCount = Mod::get()->getSavedValue<int>("search-enjLowCount", 0);
    enjHighCount = Mod::get()->getSavedValue<int>("search-enjHighCount", 0);
    enjLow = Mod::get()->getSavedValue<float>("search-enjLow", 0);
    enjHigh = Mod::get()->getSavedValue<float>("search-enjHigh", highestEnjoyment);
    sortOptionIndex = Mod::get()->getSavedValue<int>("search-sortOptionIndex", 0);
    sortDirectionIndex = Mod::get()->getSavedValue<int>("search-sortDirectionIndex", 0);
    completed = Mod::get()->getSavedValue<bool>("search-completed", true);
    uncompleted = Mod::get()->getSavedValue<bool>("search-uncompleted", true);
    simplified = Mod::get()->getSavedValue<bool>("search-simplified", false);
}

void GDDLSearchLayer::saveSettings() {
    // save values
    Mod::get()->setSavedValue("search-name", name);
    Mod::get()->setSavedValue("search-lowTier", lowTier);
    Mod::get()->setSavedValue("search-highTier", highTier);
    Mod::get()->setSavedValue("search-difficulty", difficulty);
    Mod::get()->setSavedValue("search-creator", creator);
    Mod::get()->setSavedValue("search-song", song);
    Mod::get()->setSavedValue("search-exactName", exactName);
    Mod::get()->setSavedValue("search-removeUnrated", removeUnrated);
    Mod::get()->setSavedValue("search-removeUnratedEnj", removeUnratedEnj);
    Mod::get()->setSavedValue("search-removeRated", removeRated);
    Mod::get()->setSavedValue("search-removeRatedEnj", removeRatedEnj);
    Mod::get()->setSavedValue("search-subLowCount", subLowCount);
    Mod::get()->setSavedValue("search-subHighCount", subHighCount);
    Mod::get()->setSavedValue("search-enjLowCount", enjLowCount);
    Mod::get()->setSavedValue("search-enjHighCount", enjHighCount);
    Mod::get()->setSavedValue("search-enjLow", enjLow);
    Mod::get()->setSavedValue("search-enjHigh", enjHigh);
    Mod::get()->setSavedValue("search-sortOptionIndex", sortOptionIndex);
    Mod::get()->setSavedValue("search-sortDirectionIndex", sortDirectionIndex);
    Mod::get()->setSavedValue("search-completed", completed);
    Mod::get()->setSavedValue("search-uncompleted", uncompleted);
    Mod::get()->setSavedValue("search-simplified", simplified);
}

void GDDLSearchLayer::requestSearchPage(int requestedPage, GDDLBrowserLayer *callbackObject) {
    // check whether the cache already contains results for this query
    if (requestedPage < 1) {
        requestedPage = 1;
    }
    if (!cachedResults.empty()) {
        const int maxPotentialPages = getMaxPotentialPages();
        if (maxPotentialPages < requestedPage) {
            requestedPage = maxPotentialPages;
        }
    }
    const auto [firstReadyIndex, lastReadyIndex] = getReadyRange(requestedPage);
    if (lastReadyIndex - firstReadyIndex >= 10 ||
        (onlinePagesFetched >= getOnlinePagesCount() && callbackObject != nullptr)) {
        // we have the results yaaaay (or there's no way to get more, the other check only works when it's not the first
        // request in this search)
        GJSearchObject *searchObject = makeASearchObjectFrom(firstReadyIndex, lastReadyIndex);
        handleSearchObject(searchObject, callbackObject, lastReadyIndex - firstReadyIndex);
        return;
    }
    // well, time to get them in this case :/
    const std::string request = formSearchRequest();
    requestRequestedPage = requestedPage;
    searchCallbackObject = callbackObject;
    auto req = web::WebRequest();
    searchListener.setFilter(req.get(request));
}

void GDDLSearchLayer::requestSearchFromDemonSplit(const int tier) {
    if (searching) return; // super good multithreaded code
    // save values before replacing them
    cacheValues();
    // and then
    if (tier == -1) {
        removeRated = true;
        lowTier = 0;
        highTier = 0;
    } else {
        lowTier = tier;
        highTier = tier;
    }
    completed = true;
    uncompleted = false;
    totalOnlineResults = 0;
    cachedResults.clear();
    onlinePagesFetched = 0;
    searching = true;
    requestSearchPage(1, nullptr);
}

int GDDLSearchLayer::getSearchResultsPageCount() { return getMaxPotentialPages(); }

int GDDLSearchLayer::getSearchResultsCount() { return totalOnlineResults; }

/**
 * FALLBACK ONLY IF SOMEONE GOES A PAGE TOO FAR
 */
GJSearchObject *GDDLSearchLayer::getSearchObjectForPage(const int requestedPage) {
    const auto [firstReadyIndex, lastReadyIndex] = getReadyRange(requestedPage);
    return makeASearchObjectFrom(firstReadyIndex, lastReadyIndex);
}

bool GDDLSearchLayer::isSearching() {
    return searching;
}

void GDDLSearchLayer::stopSearch() { searching = false; }

void GDDLSearchLayer::restoreValuesAfterSplit() {
    if (savedLowTier == -1) return; // there's nothing to restore
    restoreValues();
    savedLowTier = -1;
}

void GDDLSearchLayer::onExit() {
    FLAlertLayer::onExit();
    // https://github.com/B1rtek/Geode-GDDLIntegration/issues/27
    clickOffTextfields();
}

void GDDLSearchLayer::clickOffTextfields() {
    if(!simplified) { // if you do that in the simplified menu it'll crash if the full menu wasn't loaded
        nameTextfield->onClickTrackNode(false);
        creatorTextfield->onClickTrackNode(false);
        songTextfield->onClickTrackNode(false);
        tierLowTextfield->onClickTrackNode(false);
        tierHighTextfield->onClickTrackNode(false);
        enjoymentLowTextfield->onClickTrackNode(false);
        enjoymentHighTextfield->onClickTrackNode(false);
        submissionsCountHighTextfield->onClickTrackNode(false);
        submissionsCountLowTextfield->onClickTrackNode(false);
        enjSubmissionsCountHighTextfield->onClickTrackNode(false);
        enjSubmissionsCountLowTextfield->onClickTrackNode(false);
    }
    // apart from that click off the stock search bar as well - https://github.com/B1rtek/Geode-GDDLIntegration/issues/33
    // why are you dynamic casting without checking the result - Cvolton #39 #40
    const auto parent = getParent();
    if (parent == nullptr) return;
    const auto searchBar = parent->getChildByIDRecursive("search-bar");
    if (searchBar == nullptr) return;
    const auto searchBarTextInputNode = dynamic_cast<CCTextInputNode*>(searchBar);
    if (searchBarTextInputNode == nullptr) return;
    searchBarTextInputNode->onClickTrackNode(false);
}
