#include "GDDLSearchLayer.h"

#include <Geode/utils/web.hpp>

#include "RatingsManager.h"
#include "Utils.h"

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
    float aspectRatioFixX = (569.0f - winSize.width) / 2;
    float aspectRatioFixY = (320.0f - winSize.height) / 2;
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
    // info button
    const auto infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    const auto infoButton =
            CCMenuItemSpriteExtra::create(infoButtonSprite, this, menu_selector(GDDLSearchLayer::onInfo));
    m_buttonMenu->addChild(infoButton);
    m_buttonMenu->reorderChild(infoButton, 1);
    infoButton->setPosition({423.0f, -7.0f});
    // title
    const auto title = CCLabelBMFont::create("GDDL Search", "goldFont.fnt");
    m_buttonMenu->addChild(title, 1);
    title->setPosition({220.0f, -10.0f});
    title->setScale(0.7f);
    // search button
    const auto searchButtonSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn06_001.png");
    searchButtonSprite->setScale(0.7f);
    const auto searchButton =
            CCMenuItemSpriteExtra::create(searchButtonSprite, this, menu_selector(GDDLSearchLayer::onSearchClicked));
    m_buttonMenu->addChild(searchButton, 1);
    searchButton->setPosition({310.0f, -10.0f});
    // reset button
    const auto resetButtonSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn07_001.png");
    resetButtonSprite->setScale(0.7f);
    const auto resetButton =
            CCMenuItemSpriteExtra::create(resetButtonSprite, this, menu_selector(GDDLSearchLayer::onResetClicked));
    m_buttonMenu->addChild(resetButton, 1);
    resetButton->setPosition({340.0f, -10.0f});
    // interface
    loadPage();
    loadValues();
    return true;
}

void GDDLSearchLayer::loadPage() {
    // column 1
    // level name
    createLabel(m_buttonMenu, "bigFont.fnt", "Name", 110.0f, {75.0f, -37.5f});
    createTextInputNode(m_buttonMenu, nameTextfield, "bigFont.fnt", "", {80.0f, 25.0f}, {60.0f, -62.5f});
    createCheckbox(m_buttonMenu, nameExactMatchToggler, "Ex. match", 17.5f, 0.9f, {117.0f, -62.5f},
                   menu_selector(GDDLSearchLayer::onToggleExactMatch));
    // creator
    createLabel(m_buttonMenu, "bigFont.fnt", "Creator", 110.0f, {75.0f, -97.5f});
    createTextInputNode(m_buttonMenu, creatorTextfield, "bigFont.fnt", "", {110.0f, 25.0f}, {75.0f, -122.5f});
    // song name
    createLabel(m_buttonMenu, "bigFont.fnt", "Song name", 110.0f, {75.0f, -157.5f});
    createTextInputNode(m_buttonMenu, songTextfield, "bigFont.fnt", "", {110.0f, 25.0f}, {75.0f, -182.5f});
    // in-game difficulty
    createLabel(m_buttonMenu, "bigFont.fnt", "Difficulty", 110.0f, {75.0f, -217.5f});
    auto bg = createLabelForChoice(m_buttonMenu, difficultyLabel, "bigFont.fnt", "Any", 110.0f, {75.0f, -242.5f},
                                   {110.0f, 25.0f});
    createLeftRightButtonsAround(bg, {13.0f, 19.0f}, menu_selector(GDDLSearchLayer::onInGameRatingLeft),
                                 menu_selector(GDDLSearchLayer::onInGameRatingRight));

    // column 2
    // rating range
    createLabel(m_buttonMenu, "bigFont.fnt", "Tiers", 110.0f, {220.0f, -37.5f});
    createTextInputNode(m_buttonMenu, tierLowTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {182.5f, -62.5f});
    tierLowTextfield->setMaxLabelLength(2);
    tierLowTextfield->setAllowedChars("1234567890");
    createTextInputNode(m_buttonMenu, tierHighTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {257.5f, -62.5f});
    tierHighTextfield->setMaxLabelLength(2);
    tierHighTextfield->setAllowedChars("1234567890");
    createLeftRightButtonsAround(tierLowTextfield, {13.0f, 19.0f}, menu_selector(GDDLSearchLayer::onTierLowLeft),
                                 menu_selector(GDDLSearchLayer::onTierLowRight));
    createLeftRightButtonsAround(tierHighTextfield, {13.0f, 19.0f}, menu_selector(GDDLSearchLayer::onTierHighLeft),
                                 menu_selector(GDDLSearchLayer::onTierHighRight));
    createLabel(m_buttonMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -62.5f});
    // enjoyment range
    createLabel(m_buttonMenu, "bigFont.fnt", "Enjoyment rating", 110.0f, {220.0f, -97.5f});
    createTextInputNode(m_buttonMenu, enjoymentLowTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {182.5f, -122.5f});
    enjoymentLowTextfield->setMaxLabelLength(5);
    enjoymentLowTextfield->setAllowedChars("1234567890.");
    createTextInputNode(m_buttonMenu, enjoymentHighTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {257.5f, -122.5f});
    enjoymentHighTextfield->setMaxLabelLength(5);
    enjoymentHighTextfield->setAllowedChars("1234567890.");
    createLeftRightButtonsAround(enjoymentLowTextfield, {13.0f, 19.0f},
                                 menu_selector(GDDLSearchLayer::onEnjoymentLowLeft),
                                 menu_selector(GDDLSearchLayer::onEnjoymentLowRight));
    createLeftRightButtonsAround(enjoymentHighTextfield, {13.0f, 19.0f},
                                 menu_selector(GDDLSearchLayer::onEnjoymentHighLeft),
                                 menu_selector(GDDLSearchLayer::onEnjoymentHighRight));
    createLabel(m_buttonMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -122.5f});
    // submissions count
    createLabel(m_buttonMenu, "bigFont.fnt", "Submissions count", 110.0f, {220.0f, -157.5f});
    createTextInputNode(m_buttonMenu, submissionsCountLowTextfield, "bigFont.fnt", "", {35.0f, 25.0f},
                        {182.5f, -182.5f});
    submissionsCountLowTextfield->setMaxLabelLength(4);
    submissionsCountLowTextfield->setAllowedChars("1234567890");
    createTextInputNode(m_buttonMenu, submissionsCountHighTextfield, "bigFont.fnt", "", {35.0f, 25.0f},
                        {257.5f, -182.5f});
    submissionsCountHighTextfield->setMaxLabelLength(4);
    submissionsCountHighTextfield->setAllowedChars("1234567890");
    createLeftRightButtonsAround(submissionsCountLowTextfield, {13.0f, 19.0f},
                                 menu_selector(GDDLSearchLayer::onSubmissionCountLowLeft),
                                 menu_selector(GDDLSearchLayer::onSubmissionCountLowRight));
    createLeftRightButtonsAround(submissionsCountHighTextfield, {13.0f, 19.0f},
                                 menu_selector(GDDLSearchLayer::onSubmissionCountHighLeft),
                                 menu_selector(GDDLSearchLayer::onSubmissionCountHighRight));
    createLabel(m_buttonMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -182.5f});
    // enjoyment submissions count
    createLabel(m_buttonMenu, "bigFont.fnt", "Enj. submissions count", 110.0f, {220.0f, -217.5f});
    createTextInputNode(m_buttonMenu, enjSubmissionsCountLowTextfield, "bigFont.fnt", "", {35.0f, 25.0f},
                        {182.5f, -242.5f});
    enjSubmissionsCountLowTextfield->setMaxLabelLength(4);
    enjSubmissionsCountLowTextfield->setAllowedChars("1234567890");
    createTextInputNode(m_buttonMenu, enjSubmissionsCountHighTextfield, "bigFont.fnt", "", {35.0f, 25.0f},
                        {257.5f, -242.5f});
    enjSubmissionsCountHighTextfield->setMaxLabelLength(4);
    enjSubmissionsCountHighTextfield->setAllowedChars("1234567890");
    createLeftRightButtonsAround(enjSubmissionsCountLowTextfield, {13.0f, 19.0f},
                                 menu_selector(GDDLSearchLayer::onEnjSubmissionCountLowLeft),
                                 menu_selector(GDDLSearchLayer::onEnjSubmissionCountLowRight));
    createLeftRightButtonsAround(enjSubmissionsCountHighTextfield, {13.0f, 19.0f},
                                 menu_selector(GDDLSearchLayer::onEnjSubmissionCountHighLeft),
                                 menu_selector(GDDLSearchLayer::onEnjSubmissionCountHighRight));
    createLabel(m_buttonMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -242.5f});

    // column 3
    // tiers checkboxes
    createCheckbox(m_buttonMenu, noUnratedToggler, "No unrated", 17.5f, 0.9f, {325.0f, -62.5f},
                   menu_selector(GDDLSearchLayer::onToggleNoUnrated));
    createCheckbox(m_buttonMenu, noRatedToggler, "No rated", -19.5f, 0.9f, {365.0f, -62.5f},
                   menu_selector(GDDLSearchLayer::onToggleNoRated));
    createCheckbox(m_buttonMenu, completedToggler, "Completed", 17.5f, 0.9f, {405.0f, -62.5f},
                   menu_selector(GDDLSearchLayer::onToggleCompleted));
    // enjoyment rating checkboxes
    createCheckbox(m_buttonMenu, noUnratedEnjToggler, "No unr. enj.", 17.5f, 0.9f, {325.0f, -122.5f},
                   menu_selector(GDDLSearchLayer::onToggleNoUnratedEnj));
    createCheckbox(m_buttonMenu, noRatedEnjToggler, "No rated enj.", -19.5f, 0.9f, {365.0f, -122.5f},
                   menu_selector(GDDLSearchLayer::onToggleNoRatedEnj));
    createCheckbox(m_buttonMenu, uncompletedToggler, "Uncompleted", 17.5f, 0.9f, {405.0f, -122.5f},
                   menu_selector(GDDLSearchLayer::onToggleUncompleted));
    // sort by
    createLabel(m_buttonMenu, "bigFont.fnt", "Sort by", 110.0f, {365.0f, -157.5f});
    bg = createLabelForChoice(m_buttonMenu, sortByLabel, "bigFont.fnt", "ID", 110.0f, {365.0f, -182.5f},
                              {110.0f, 25.0f});
    createLeftRightButtonsAround(bg, {13.0f, 19.0f}, menu_selector(GDDLSearchLayer::onSortByLeft),
                                 menu_selector(GDDLSearchLayer::onSortByRight));
    // sort direction
    createLabel(m_buttonMenu, "bigFont.fnt", "Sort direction", 110.0f, {365.0f, -217.5f});
    bg = createLabelForChoice(m_buttonMenu, sortDirectionLabel, "bigFont.fnt", "Ascending", 110.0f, {365.0f, -242.5f},
                              {110.0f, 25.0f});
    createLeftRightButtonsAround(bg, {13.0f, 19.0f}, menu_selector(GDDLSearchLayer::onSortDirectionLeft),
                                 menu_selector(GDDLSearchLayer::onSortDirectionRight));
}

void GDDLSearchLayer::loadValues() {
    page = 1;
    nameTextfield->setString(name.c_str());
    setNumberWithDefZeroTextfield(lowTier, tierLowTextfield);
    setNumberWithDefZeroTextfield(highTier, tierHighTextfield);
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
    setNumberWithDefZeroTextfield(subLowCount, submissionsCountLowTextfield);
    setNumberWithDefZeroTextfield(subHighCount, submissionsCountHighTextfield);
    setNumberWithDefZeroTextfield(enjLowCount, enjSubmissionsCountLowTextfield);
    setNumberWithDefZeroTextfield(enjHighCount, enjSubmissionsCountHighTextfield);
    setNumberFloatTextfield(enjLow, enjoymentLowTextfield);
    setNumberFloatTextfield(enjHigh, enjoymentHighTextfield);
    setSortDirectionLabel();
    setSortByLabel();
}

void GDDLSearchLayer::saveValues() {
    // fields with a set of choices don't have to be saved
    name = nameTextfield->getString();
    lowTier = getNumberTextfieldValue(tierLowTextfield);
    highTier = getNumberTextfieldValue(tierHighTextfield);
    creator = creatorTextfield->getString();
    song = songTextfield->getString();
    exactName = nameExactMatchToggler->isToggled();
    removeUnrated = noUnratedToggler->isToggled();
    removeUnratedEnj = noUnratedEnjToggler->isToggled();
    removeRated = noRatedToggler->isToggled();
    removeRatedEnj = noRatedEnjToggler->isToggled();
    completed = completedToggler->isToggled();
    uncompleted = uncompletedToggler->isToggled();
    subLowCount = getNumberTextfieldValue(submissionsCountLowTextfield);
    subHighCount = getNumberTextfieldValue(submissionsCountHighTextfield);
    enjLowCount = getNumberTextfieldValue(enjSubmissionsCountLowTextfield);
    enjHighCount = getNumberTextfieldValue(enjSubmissionsCountHighTextfield);
    enjLow = getFloatTextfieldValue(enjoymentLowTextfield);
    enjHigh = getFloatTextfieldValue(enjoymentHighTextfield);
}

void GDDLSearchLayer::resetValues() {
    page = 1;
    nameTextfield->setString("");
    setNumberWithDefZeroTextfield(0, tierLowTextfield);
    setNumberWithDefZeroTextfield(0, tierHighTextfield);
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
    setNumberWithDefZeroTextfield(0, submissionsCountLowTextfield);
    setNumberWithDefZeroTextfield(0, submissionsCountHighTextfield);
    setNumberWithDefZeroTextfield(0, enjSubmissionsCountLowTextfield);
    setNumberWithDefZeroTextfield(0, enjSubmissionsCountHighTextfield);
    setNumberFloatTextfield(0.0f, enjoymentLowTextfield);
    setNumberFloatTextfield(highestEnjoyment, enjoymentHighTextfield);
    sortDirectionIndex = 0;
    setSortDirectionLabel();
    sortOptionIndex = 0;
    setSortByLabel();
}

void GDDLSearchLayer::onClose(CCObject *sender) {
    saveValues();
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}
TodoReturn GDDLSearchLayer::keyBackClicked() {
    saveValues();
    FLAlertLayer::keyBackClicked();
}

void GDDLSearchLayer::onInfo(CCObject *sender) {
    std::string infoContent =
            "<co>Name - Ex. match</c> - will <cy>only</c> search for levels with <cy>exactly</c> matching "
            "names\n<co>Difficulty</c> - <cy>in-game</c> <cr>demon</c> difficulty\n<co>Enj./Submissions count</c> - "
            "the amount of <cj>enjoyment</c>/<cp>difficulty tier</c> submissions for a level\n<co>No unrated/No rated "
            "(Enj)</c> - <cy>only</c> search for levels <cg>with</c>(<cr>out</c>) a <cp>tier</c>/<cj>enjoyment</c> "
            "rating\n<cb>All</c> tier and submission ranges are <cy>inclusive</c> from <cy>both</c> sides";
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

std::string GDDLSearchLayer::addStringToRequest(const std::string &paramName, std::string value) {
    if (value.empty()) {
        return "";
    }
    return "&" + paramName + "=" + urlEncodeString(value);
}

std::string GDDLSearchLayer::addBoolToRequest(const std::string &paramName, bool value) {
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
    request += addValueToRequest("difficulty", difficulty, 5);
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
std::vector<int> GDDLSearchLayer::parseResponse(std::string response) {
    std::vector<int> results;
    json responseJson = json::parse(response);
    totalOnlineResults = responseJson["total"];
    json levelList = responseJson["levels"];
    for (auto element: levelList) {
        results.push_back(element["LevelID"]);
    }
    return results;
}

std::vector<int> GDDLSearchLayer::filterResults(std::vector<int> ids, LevelCompleteness completionStatus) {
    std::set<int> setOfIds = Utils::copyVectorToSet(ids);
    std::vector<int> filteredList;
    if (completionStatus != ANY1) {
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
        if (completionStatus == COMPLETED1) {
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
    const int searchResultsCount = cachedResults.size();
    const int correction = searchResultsCount % 10 == 0 ? 0 : 1;
    return searchResultsCount / 10 + correction;
}

GJSearchObject *GDDLSearchLayer::makeASearchObjectFrom(const int firstIndex, const int lastIndex) {
    std::string requestString;
    for (int i = firstIndex; i < lastIndex; i++) {
        requestString += std::to_string(cachedResults[i]) + ',';
    }
    if (!requestString.empty()) {
        requestString.pop_back();
    }
    requestString += "&gameVersion=22";
    return GJSearchObject::create(SearchType::Type19, requestString);
}

void GDDLSearchLayer::appendFetchedResults(std::string response) {
    std::vector<int> parsedResponse = parseResponse(response);
    std::vector<int> filteredResponse = filterResults(parsedResponse, completeness);
    for (auto element: filteredResponse) {
        cachedResults.push_back(element);
    }
    onlinePagesFetched++;
}

std::pair<int, int> GDDLSearchLayer::getReadyRange(int requestedPage) {
    const int firstIndex = (requestedPage - 1) * 10;
    const int lastIndex = std::min(firstIndex + 10, (int) cachedResults.size()); // last index + 1
    return {firstIndex, lastIndex};
}

void GDDLSearchLayer::handleSearchObject(GJSearchObject *searchObject, std::function<void(GJSearchObject *)> callback) {
    if(callback != nullptr) { // search continues
        callback(searchObject);
    } else { // new search
        const auto listLayer = LevelBrowserLayer::create(searchObject);
        cocos::switchToScene(listLayer);
    }
}

void GDDLSearchLayer::createLabel(CCLayer *parent, std::string font, std::string text, int maxWidth, CCPoint position,
                                  int zOrder) {
    auto label = CCLabelBMFont::create(text.c_str(), font.c_str());
    parent->addChild(label, zOrder);
    label->setPosition(position);
    scaleLabelToWidth(label, maxWidth);
}

CCScale9Sprite *GDDLSearchLayer::createLabelForChoice(CCLayer *parent, CCLabelBMFont *&label, std::string font,
                                                      std::string placeholder, int maxWidth, CCPoint position,
                                                      CCPoint bgSize, int zOrder) {
    label = CCLabelBMFont::create(placeholder.c_str(), font.c_str());
    parent->addChild(label, zOrder);
    label->setPosition(position);
    scaleLabelToWidth(label, maxWidth);
    const auto bg = CCScale9Sprite::create("square02_small.png");
    parent->addChild(bg, zOrder + 1);
    bg->setContentSize(bgSize);
    bg->setScale(0.5f);
    bg->setContentSize(bg->getContentSize() / 0.5f);
    bg->setPosition(position);
    bg->setOpacity(100);
    return bg;
}
void GDDLSearchLayer::scaleLabelToWidth(CCLabelBMFont *&label, float maxWidth) {
    const float scale =
            0.6f * label->getContentSize().width > maxWidth ? maxWidth / label->getContentSize().width : 0.6f;
    label->setScale(scale);
}

void GDDLSearchLayer::createTextInputNode(CCLayer *parent, CCTextInputNode *&textfield, std::string font,
                                          std::string placeholder, CCPoint bgSize, CCPoint position, int zOrder) {
    const auto bg = CCScale9Sprite::create("square02_small.png");
    parent->addChild(bg, zOrder);
    bg->setContentSize(bgSize);
    bg->setScale(0.5f);
    bg->setContentSize(bg->getContentSize() / 0.5f);
    bg->setPosition(position);
    bg->setOpacity(100);
    textfield = CCTextInputNode::create(bgSize.x, bgSize.y, placeholder.c_str(), font.c_str());
    parent->addChild(textfield, zOrder + 1);
    textfield->setPosition(position);
    textfield->setMaxLabelLength(32);
    textfield->setMaxLabelScale(0.7f);
}

void GDDLSearchLayer::createLeftRightButtonsAround(CCNode *object, CCPoint size, SEL_MenuHandler leftCallback,
                                                   SEL_MenuHandler rightCallback, int zOrder) {
    // left
    const CCPoint positionLeft =
            object->getPosition() -
            CCPoint(object->getContentSize().width * object->getScale() / 2 + size.x / 2 + 0.5f, -1.0f);
    const auto leftButtonSprite = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
    leftButtonSprite->setScale(0.8f);
    const auto leftButton = CCMenuItemSpriteExtra::create(leftButtonSprite, this, leftCallback);
    object->getParent()->addChild(leftButton, zOrder);
    leftButton->setPosition(positionLeft);
    leftButton->setContentSize(size);
    // right
    const CCPoint positionRight = positionLeft + CCPoint(object->getContentSize().width * object->getScale() +
                                                                 leftButton->getContentSize().width + 2.5f,
                                                         0.0f); // why is this not symmetrical wtf
    const auto rightButtonSprite = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
    rightButtonSprite->setScale(0.8f);
    const auto rightButton = CCMenuItemSpriteExtra::create(rightButtonSprite, this, rightCallback);
    object->getParent()->addChild(rightButton, zOrder);
    rightButton->setPosition(positionRight);
    rightButton->setContentSize(size);
}

void GDDLSearchLayer::createCheckbox(CCLayer *parent, CCMenuItemToggler *&toggler, std::string label, float labelOffset,
                                     float scale, CCPoint position, SEL_MenuHandler callback, int zOrder) {
    toggler = CCMenuItemToggler::createWithStandardSprites(this, callback, scale);
    parent->addChild(toggler, zOrder);
    toggler->setPosition(position);
    const auto toggleLabel = CCLabelBMFont::create(label.c_str(), "bigFont.fnt");
    parent->addChild(toggleLabel, zOrder);
    toggleLabel->setPosition({toggler->getPositionX(), toggler->getPositionY() - labelOffset});
    const float maxWidth = toggler->getContentSize().width * scale * 2.0f;
    const float labelScale = 0.3f * toggleLabel->getContentSize().width > maxWidth
                                     ? maxWidth / toggleLabel->getContentSize().width
                                     : 0.3f;
    toggleLabel->setScale(labelScale);
}

float GDDLSearchLayer::calculateNewFloat(float currentValue, bool increase, float lowerbound, float upperbound) {
    int newValue = currentValue + (increase ? 1.0f : -1.0f);
    if (std::abs(currentValue - std::floor(currentValue)) >= 0.01f) {
        newValue = increase ? std::floor(currentValue) + 1 : std::floor(currentValue);
    }
    if (newValue < lowerbound)
        newValue = upperbound;
    if (newValue > upperbound)
        newValue = lowerbound;
    return newValue;
}

void GDDLSearchLayer::onToggleExactMatch(CCObject *sender) {
    exactName = !dynamic_cast<CCMenuItemToggler *>(sender)->isOn();
}

void GDDLSearchLayer::onInGameRatingLeft(CCObject *sender) {
    difficulty--;
    if (difficulty < 0)
        difficulty = demonDifficulties.size() - 1;
    setDifficultyLabel();
}

void GDDLSearchLayer::onInGameRatingRight(CCObject *sender) {
    difficulty++;
    if (difficulty > demonDifficulties.size() - 1)
        difficulty = 0;
    setDifficultyLabel();
}

void GDDLSearchLayer::onTierLowLeft(CCObject *sender) {
    int newValue = getNumberTextfieldValue(tierLowTextfield) - 1;
    if (newValue < 0)
        newValue = highestTier;
    setNumberWithDefZeroTextfield(newValue, tierLowTextfield);
}
void GDDLSearchLayer::onTierLowRight(CCObject *sender) {
    int newValue = getNumberTextfieldValue(tierLowTextfield) + 1;
    if (newValue > highestTier)
        newValue = 0;
    setNumberWithDefZeroTextfield(newValue, tierLowTextfield);
}

void GDDLSearchLayer::onTierHighLeft(CCObject *sender) {
    int newValue = getNumberTextfieldValue(tierHighTextfield) - 1;
    if (newValue < 0)
        newValue = highestTier;
    setNumberWithDefZeroTextfield(newValue, tierHighTextfield);
}

void GDDLSearchLayer::onTierHighRight(CCObject *sender) {
    int newValue = getNumberTextfieldValue(tierHighTextfield) + 1;
    if (newValue > highestTier)
        newValue = 0;
    setNumberWithDefZeroTextfield(newValue, tierHighTextfield);
}

void GDDLSearchLayer::onEnjoymentLowLeft(CCObject *sender) {
    float currentValue = getFloatTextfieldValue(enjoymentLowTextfield);
    int newValue = calculateNewFloat(currentValue, false, 0.0f, highestEnjoyment);
    setNumberFloatTextfield(newValue, enjoymentLowTextfield);
}

void GDDLSearchLayer::onEnjoymentLowRight(CCObject *sender) {
    int currentValue = getNumberTextfieldValue(enjoymentLowTextfield);
    int newValue = calculateNewFloat(currentValue, true, 0.0f, highestEnjoyment);
    setNumberFloatTextfield(newValue, enjoymentLowTextfield);
}

void GDDLSearchLayer::onEnjoymentHighLeft(CCObject *sender) {
    float currentValue = getFloatTextfieldValue(enjoymentHighTextfield);
    int newValue = calculateNewFloat(currentValue, false, 0.0f, highestEnjoyment);
    setNumberFloatTextfield(newValue, enjoymentHighTextfield);
}

void GDDLSearchLayer::onEnjoymentHighRight(CCObject *sender) {
    float currentValue = getFloatTextfieldValue(enjoymentHighTextfield);
    int newValue = calculateNewFloat(currentValue, true, 0.0f, highestEnjoyment);
    setNumberFloatTextfield(newValue, enjoymentHighTextfield);
}

void GDDLSearchLayer::onSubmissionCountLowLeft(CCObject *sender) {
    int newValue = getNumberTextfieldValue(submissionsCountLowTextfield) - 1;
    if (newValue < 0)
        newValue = 0;
    setNumberWithDefZeroTextfield(newValue, submissionsCountLowTextfield);
}

void GDDLSearchLayer::onSubmissionCountLowRight(CCObject *sender) {
    int newValue = getNumberTextfieldValue(submissionsCountLowTextfield) + 1;
    if (newValue > maxSubmissions)
        newValue = maxSubmissions;
    setNumberWithDefZeroTextfield(newValue, submissionsCountLowTextfield);
}

void GDDLSearchLayer::onSubmissionCountHighLeft(CCObject *sender) {
    int newValue = getNumberTextfieldValue(submissionsCountHighTextfield) - 1;
    if (newValue < 0)
        newValue = 0;
    setNumberWithDefZeroTextfield(newValue, submissionsCountHighTextfield);
}

void GDDLSearchLayer::onSubmissionCountHighRight(CCObject *sender) {
    int newValue = getNumberTextfieldValue(submissionsCountHighTextfield) + 1;
    if (newValue > maxSubmissions)
        newValue = maxSubmissions;
    setNumberWithDefZeroTextfield(newValue, submissionsCountHighTextfield);
}

void GDDLSearchLayer::onEnjSubmissionCountLowLeft(CCObject *sender) {
    int newValue = getNumberTextfieldValue(enjSubmissionsCountLowTextfield) - 1;
    if (newValue < 0)
        newValue = 0;
    setNumberWithDefZeroTextfield(newValue, enjSubmissionsCountLowTextfield);
}

void GDDLSearchLayer::onEnjSubmissionCountLowRight(CCObject *sender) {
    int newValue = getNumberTextfieldValue(enjSubmissionsCountLowTextfield) + 1;
    if (newValue > maxSubmissions)
        newValue = maxSubmissions;
    setNumberWithDefZeroTextfield(newValue, enjSubmissionsCountLowTextfield);
}

void GDDLSearchLayer::onEnjSubmissionCountHighLeft(CCObject *sender) {
    int newValue = getNumberTextfieldValue(enjSubmissionsCountHighTextfield) - 1;
    if (newValue < 0)
        newValue = 0;
    setNumberWithDefZeroTextfield(newValue, enjSubmissionsCountHighTextfield);
}

void GDDLSearchLayer::onEnjSubmissionCountHighRight(CCObject *sender) {
    int newValue = getNumberTextfieldValue(enjSubmissionsCountHighTextfield) + 1;
    if (newValue > maxSubmissions)
        newValue = maxSubmissions;
    setNumberWithDefZeroTextfield(newValue, enjSubmissionsCountHighTextfield);
}

void GDDLSearchLayer::onToggleNoUnrated(CCObject *sender) {
    if (!noUnratedToggler->isOn()) {
        noRatedToggler->toggle(false);
    }
}

void GDDLSearchLayer::onToggleNoRated(CCObject *sender) {
    if (!noRatedToggler->isOn()) {
        noUnratedToggler->toggle(false);
    }
}

void GDDLSearchLayer::onToggleNoUnratedEnj(CCObject *sender) {
    if (!noUnratedEnjToggler->isOn()) {
        noRatedEnjToggler->toggle(false);
    }
}

void GDDLSearchLayer::onToggleNoRatedEnj(CCObject *sender) {
    if (!noRatedEnjToggler->isOn()) {
        noUnratedEnjToggler->toggle(false);
    }
}

void GDDLSearchLayer::onToggleCompleted(CCObject *sender) {
    // do nothing
}

void GDDLSearchLayer::onToggleUncompleted(CCObject *sender) {
    // do nothing
}

void GDDLSearchLayer::onSortByLeft(CCObject *sender) {
    sortOptionIndex--;
    if (sortOptionIndex < 0)
        sortOptionIndex = sort.size() - 1;
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
        sortDirectionIndex = sortDirection.size() - 1;
    setSortDirectionLabel();
}

void GDDLSearchLayer::onSortDirectionRight(CCObject *sender) {
    sortDirectionIndex++;
    if (sortDirectionIndex > sortDirection.size() - 1)
        sortDirectionIndex = 0;
    setSortDirectionLabel();
}

void GDDLSearchLayer::onSearchClicked(CCObject *sender) {
    saveValues();
    totalOnlineResults = 0;
    cachedResults.clear();
    onlinePagesFetched = 0;
    searching = true;
    requestSearchPage(1, nullptr);
}

void GDDLSearchLayer::onResetClicked(CCObject *sender) { resetValues(); }

void GDDLSearchLayer::setNumberWithDefZeroTextfield(int value, CCTextInputNode *&textfield) {
    if (value != 0) {
        textfield->setString(std::to_string(value).c_str());
    } else {
        textfield->setString("");
    }
}
void GDDLSearchLayer::setNumberFloatTextfield(float value, CCTextInputNode *&textfield) {
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
    scaleLabelToWidth(difficultyLabel, 110.0f);
}

void GDDLSearchLayer::setSortByLabel() {
    sortByLabel->setString(sortBy[sortOptionIndex].c_str());
    scaleLabelToWidth(sortByLabel, 110.0f);
}

void GDDLSearchLayer::setSortDirectionLabel() {
    sortDirectionLabel->setString(orderDirection[sortDirectionIndex].c_str());
    scaleLabelToWidth(sortDirectionLabel, 110.0f);
}

int GDDLSearchLayer::getNumberTextfieldValue(CCTextInputNode *&textfield) {
    if (textfield->getString().empty())
        return 0;
    return std::stoi(textfield->getString());
}

float GDDLSearchLayer::getFloatTextfieldValue(CCTextInputNode *&textfield) {
    if (textfield->getString().empty())
        return 0;
    return std::stof(textfield->getString());
}

GDDLSearchLayer *GDDLSearchLayer::create() {
    const auto newLayer = new GDDLSearchLayer();
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

void GDDLSearchLayer::loadSettings() {}

void GDDLSearchLayer::saveSettings() {}

void GDDLSearchLayer::requestSearchPage(int requestedPage, std::function<void(GJSearchObject *)> callback) {
    // check whether the cache already contains results for this query
    if (requestedPage < 1) {
        requestedPage = 1;
    }
    if (!cachedResults.empty()) {
        const int maxPotentialPages = getMaxPotentialPages();
        if (maxPotentialPages > requestedPage) {
            requestedPage = maxPotentialPages;
        }
    }
    std::pair<int, int> readyRange = getReadyRange(requestedPage);
    if (readyRange.second - readyRange.first >= 10) { // we have the results yaaaay
        GJSearchObject* searchObject = makeASearchObjectFrom(readyRange.first, readyRange.second);
        handleSearchObject(searchObject, callback);
    }
    // well, time to get them in this case :/
    std::string request = formSearchRequest();
    web::AsyncWebRequest()
            .fetch(request)
            .text()
            .then([requestedPage, &callback](std::string const &response) {
                // append results
                appendFetchedResults(response);
                // test if there's enough of them
                std::pair<int, int> readyRange = getReadyRange(requestedPage);
                while(readyRange.second - readyRange.first < 10 && onlinePagesFetched < getMaxPotentialPages()) {
                    // not enough? fetch some more!
                    auto anotherResponse = web::fetch(formSearchRequest());
                    appendFetchedResults(response);
                    readyRange = getReadyRange(requestedPage);
                    // until it runs out of pages or sth
                }
                // and then call the callback
                GJSearchObject* searchObject = makeASearchObjectFrom(readyRange.first, readyRange.second);
                handleSearchObject(searchObject, callback);
            })
            .expect([](std::string const &error) {
                FLAlertLayer::create("GDDL Search",
                                     "Search failed - either you're disconnected from the internet or the server did "
                                     "something wrong...",
                                     "OK")
                        ->show();
            });
}

int GDDLSearchLayer::getSearchResultsPageCount() { return getMaxPotentialPages(); }

int GDDLSearchLayer::getSearchResultsCount() { return totalOnlineResults; }

bool GDDLSearchLayer::isSearching() {
    return searching;
}

void GDDLSearchLayer::stopSearch() {
    searching = false;
}
