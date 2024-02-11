#include "GDDLSearchLayer.h"


#include "RatingsManager.h"
bool GDDLSearchLayer::init() {
    if (!FLAlertLayer::init(75))
        return false;

    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    // background
    const auto bg = CCScale9Sprite::create("GJ_square01.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({winSize.width / 2, winSize.height / 2});
    bg->setID("gddl-demon-search-popup"_spr);
    m_mainLayer->addChild(bg, -1);
    // menu with the main layout
    m_buttonMenu = CCMenu::create();
    m_buttonMenu->setContentSize({winSize.width, winSize.height});
    m_buttonMenu->setPosition({74.5f, 295.0f});
    m_buttonMenu->setID("gddl-demon-search-menu"_spr);
    m_mainLayer->addChild(m_buttonMenu, 0);
    // close button
    const auto closeButtonSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    const auto closeButton = CCMenuItemSpriteExtra::create(closeButtonSprite, this, menu_selector(GDDLSearchLayer::onClose));
    m_buttonMenu->addChild(closeButton, 1);
    closeButton->setPosition({0.0f, 0.0f});
    // info button
    const auto infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    const auto infoButton = CCMenuItemSpriteExtra::create(infoButtonSprite, this, menu_selector(GDDLSearchLayer::onInfo));
    m_buttonMenu->addChild(infoButton);
    m_buttonMenu->reorderChild(infoButton, 1);
    infoButton->setPosition({413.0f, -7.0f});
    // title
    const auto title = CCLabelBMFont::create("GDDL Search", "goldFont.fnt");
    m_mainLayer->addChild(title, 1);
    title->setPosition({284.5f, 287.0f});
    title->setScale(0.7f);
    loadPage();
    // title
    // const auto titleContainer = CCMenu::create();
    // titleContainer->setLayout(RowLayout::create());
    // titleContainer->setContentSize({200.0f, 30.0f});
    // const auto title = CCLabelBMFont::create("GDDL Search", "goldFont.fnt");
    // title->setID("gddl-demon-search-title"_spr);
    // titleContainer->addChild(title);
    // titleContainer->reorderChild(title, 0);
    // // the (i) button
    // const auto iButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    // iButtonSprite->setScale(0.5f);
    // const auto iButton = CCMenuItemSpriteExtra::create(iButtonSprite, this, menu_selector(GDDLSearchLayer::onInfo));
    // iButton->setScale(0.4f);
    // iButton->setContentSize({10.0f, 10.0f});
    // titleContainer->addChild(iButton);
    // titleContainer->updateLayout();
    // m_buttonMenu->addChild(titleContainer);
    // m_buttonMenu->reorderChild(titleContainer, 0);
    // // tier buttons
    // for (int row = 0; row < 5; row++) {
    //     const auto rowNode = CCMenu::create();
    //     rowNode->setLayout(RowLayout::create()->setGap(5.0f));
    //     rowNode->setContentSize({popupSize.x - 10.0f, 20.0f});
    //     for (int column = 0; column < 7; column++) {
    //         const auto tierNode = createTierNode(row + 1 + column * 5);
    //         rowNode->addChild(tierNode);
    //     }
    //     rowNode->updateLayout();
    //     m_buttonMenu->addChild(rowNode);
    //     m_buttonMenu->reorderChild(rowNode, row + 1);
    // }
    // // togglers - container
    // const auto rowNode = CCMenu::create();
    // rowNode->setLayout(RowLayout::create()->setGap(5.0f));
    // rowNode->setContentSize({popupSize.x - 10.0f, 35.0f});
    // // unknown tier
    // const auto unknownButton = createTierNode(-1);
    // rowNode->addChild(unknownButton);
    // // togglers - checkboxes
    // const std::string ids[2] = {"completed", "uncompleted"};
    // const std::string labels[2] = {"Completed", "Uncompleted"};
    // for (int i = 0; i < 2; i++) {
    //     const auto togglerNode = createCheckboxNode(ids[i], labels[i]);
    //     rowNode->addChild(togglerNode);
    //     rowNode->reorderChild(togglerNode, i);
    // }
    // rowNode->updateLayout();
    // m_buttonMenu->addChild(rowNode);
    // m_buttonMenu->reorderChild(rowNode, 9);
    // // ok button
    // const auto spr = ButtonSprite::create("OK");
    // const auto okButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GDDLSearchLayer::onClose));
    // okButton->setID("gddl-demon-split-ok"_spr);
    // m_buttonMenu->addChild(okButton);
    // m_buttonMenu->reorderChild(okButton, 10);
    // m_buttonMenu->updateLayout();
    //
    // // fix the iButton :tm: placement
    // iButton->setPosition({iButton->getPositionX(), 15.0f});
    return true;
}

void GDDLSearchLayer::loadPage() {
    // column 1
    // level name
    createLabel(m_buttonMenu, "bigFont.fnt", "Name", 110.0f, {75.0f, -52.5f});
    createTextInputNode(m_buttonMenu, nameTextfield, "bigFont.fnt", "", {80.0f, 25.0f}, {60.0f, -77.5f});
    createCheckbox(m_buttonMenu, nameExactMatchToggler, "Ex. match", 17.5f, 0.9f, {117.0f, -77.5f}, menu_selector(GDDLSearchLayer::onToggleExactMatch));
    // creator
    createLabel(m_buttonMenu, "bigFont.fnt", "Creator", 110.0f, {75.0f, -112.5f});
    createTextInputNode(m_buttonMenu, creatorTextfield, "bigFont.fnt", "", {110.0f, 25.0f}, {75.0f, -137.5f});
    // song name
    createLabel(m_buttonMenu, "bigFont.fnt", "Song name", 110.0f, {75.0f, -172.5f});
    createTextInputNode(m_buttonMenu, songTextfield, "bigFont.fnt", "", {110.0f, 25.0f}, {75.0f, -197.5f});
    // in-game difficulty
    createLabel(m_buttonMenu, "bigFont.fnt", "Difficulty", 110.0f, {75.0f, -232.5f});
    auto bg = createLabelForChoice(m_buttonMenu, difficultyLabel, "bigFont.fnt", "Any", 110.0f, {75.0f, -257.5f}, {110.0f, 25.0f});
    createLeftRightButtonsAround(bg, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onInGameRatingLeft), menu_selector(GDDLSearchLayer::onInGameRatingRight));

    // column 2
    // rating range
    createLabel(m_buttonMenu, "bigFont.fnt", "Tiers", 110.0f, {220.0f, -52.5f});
    createTextInputNode(m_buttonMenu, tierLowTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {182.5f, -77.5f});
    createTextInputNode(m_buttonMenu, tierHighTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {257.5f, -77.5f});
    createLeftRightButtonsAround(tierLowTextfield, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onTierLowLeft), menu_selector(GDDLSearchLayer::onTierLowRight));
    createLeftRightButtonsAround(tierHighTextfield, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onTierHighLeft), menu_selector(GDDLSearchLayer::onTierHighRight));
    createLabel(m_buttonMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -77.5f});
    // enjoyment range
    createLabel(m_buttonMenu, "bigFont.fnt", "Enjoyment rating", 110.0f, {220.0f, -112.5f});
    createTextInputNode(m_buttonMenu, enjoymentLowTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {182.5f, -137.5f});
    createTextInputNode(m_buttonMenu, enjoymentHighTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {257.5f, -137.5f});
    createLeftRightButtonsAround(enjoymentLowTextfield, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onEnjoymentLowLeft), menu_selector(GDDLSearchLayer::onEnjoymentLowRight));
    createLeftRightButtonsAround(enjoymentHighTextfield, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onEnjoymentHighLeft), menu_selector(GDDLSearchLayer::onEnjoymentHighRight));
    createLabel(m_buttonMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -137.5f});
    // submissions count
    createLabel(m_buttonMenu, "bigFont.fnt", "Submissions count", 110.0f, {220.0f, -172.5f});
    createTextInputNode(m_buttonMenu, submissionsCountLowTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {182.5f, -197.5f});
    createTextInputNode(m_buttonMenu, submissionsCountHighTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {257.5f, -197.5f});
    createLeftRightButtonsAround(submissionsCountLowTextfield, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onSubmissionCountLowLeft), menu_selector(GDDLSearchLayer::onSubmissionCountLowRight));
    createLeftRightButtonsAround(submissionsCountHighTextfield, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onSubmissionCountHighLeft), menu_selector(GDDLSearchLayer::onSubmissionCountHighRight));
    createLabel(m_buttonMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -197.5f});
    // enjoyment submissions count
    createLabel(m_buttonMenu, "bigFont.fnt", "Enj. submissions count", 110.0f, {220.0f, -232.5f});
    createTextInputNode(m_buttonMenu, submissionsCountLowTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {182.5f, -257.5f});
    createTextInputNode(m_buttonMenu, submissionsCountHighTextfield, "bigFont.fnt", "", {35.0f, 25.0f}, {257.5f, -257.5f});
    createLeftRightButtonsAround(submissionsCountLowTextfield, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onEnjSubmissionCountLowLeft), menu_selector(GDDLSearchLayer::onEnjSubmissionCountLowRight));
    createLeftRightButtonsAround(submissionsCountHighTextfield, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onEnjSubmissionCountHighLeft), menu_selector(GDDLSearchLayer::onEnjSubmissionCountHighRight));
    createLabel(m_buttonMenu, "chatFont.fnt", "to", 30.0f, {220.0f, -257.5f});

    // column 3
    // tiers checkboxes
    createCheckbox(m_buttonMenu, noUnratedToggler, "No unrated", 17.5f, 0.9f, {305.0f, -77.5f}, menu_selector(GDDLSearchLayer::onToggleNoUnrated));
    createCheckbox(m_buttonMenu, noRatedToggler, "No rated", 17.5f, 0.9f, {405.0f, -77.5f}, menu_selector(GDDLSearchLayer::onToggleNoRated));
    // enjoyment rating checkboxes
    createCheckbox(m_buttonMenu, noUnratedEnjToggler, "No unr. enj.", 17.5f, 0.9f, {305.0f, -137.5f}, menu_selector(GDDLSearchLayer::onToggleNoUnratedEnj));
    createCheckbox(m_buttonMenu, noRatedEnjToggler, "No rated enj.", 17.5f, 0.9f, {405.0f, -137.5f}, menu_selector(GDDLSearchLayer::onToggleNoRatedEnj));
    // sort by
    createLabel(m_buttonMenu, "bigFont.fnt", "Sort by", 110.0f, {355.0f, -172.5f});
    bg = createLabelForChoice(m_buttonMenu, sortByLabel, "bigFont.fnt", "ID", 110.0f, {355.0f, -197.5f}, {110.0f, 25.0f});
    createLeftRightButtonsAround(bg, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onSortByLeft), menu_selector(GDDLSearchLayer::onSortByRight));
    // sort direction
    createLabel(m_buttonMenu, "bigFont.fnt", "Sort direction", 110.0f, {355.0f, -232.5f});
    bg = createLabelForChoice(m_buttonMenu, sortDirectionLabel, "bigFont.fnt", "Ascending", 110.0f, {355.0f, -257.5f}, {110.0f, 25.0f});
    createLeftRightButtonsAround(bg, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onSortDirectionLeft), menu_selector(GDDLSearchLayer::onSortDirectionRight));
}

void GDDLSearchLayer::onClose(CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLSearchLayer::onInfo(CCObject *sender) {
    FLAlertLayer::create("GDDL Search", "Select the <cg>completed</c>/<cr>uncompleted</c> checkboxes and click a <co>button</c> to search for a corresponding <cy>tier</c>, click the <cp>?</c> button to search for levels that are <cb>not yet rated</c>", "OK")->show();
}

void GDDLSearchLayer::onSearchOptionSelected(CCObject *sender) {} // do nothing lmao

void GDDLSearchLayer::onTierSearch(CCObject *sender) {
    auto *senderNode = dynamic_cast<CCNode *>(sender);
    const std::string tierStr = senderNode->getID();
    const int tierNumber = std::stoi(tierStr.substr(12, tierStr.size() - 10));
    const bool searchForCompleted =
            dynamic_cast<CCMenuItemToggler *>(m_buttonMenu->getChildByIDRecursive("toggler-completed"))->isToggled();
    const bool searchForUncompleted =
            dynamic_cast<CCMenuItemToggler *>(m_buttonMenu->getChildByIDRecursive("toggler-uncompleted"))->isToggled();
    TierSearchType searchType = ANY;
    if (searchForCompleted != searchForUncompleted) {
        searchType = searchForCompleted ? COMPLETED : UNCOMPLETED;
    }
    RatingsManager::setupSearch(tierNumber, searchType);
    const auto listLayer = LevelBrowserLayer::create(RatingsManager::getSearchPage(1));
    cocos::switchToScene(listLayer);
}

void GDDLSearchLayer::createLabel(CCLayer *parent, std::string font, std::string text, int maxWidth, CCPoint position,
                                  int zOrder) {
    const auto label = CCLabelBMFont::create(text.c_str(), font.c_str());
    parent->addChild(label, zOrder);
    label->setPosition(position);
    const float scale =
            0.6f * label->getContentSize().width > maxWidth ? maxWidth / label->getContentSize().width : 0.6f;
    label->setScale(scale);
}

CCScale9Sprite* GDDLSearchLayer::createLabelForChoice(CCLayer *parent, CCLabelBMFont *&label, std::string font, std::string placeholder,
                                            int maxWidth, CCPoint position, CCPoint bgSize, int zOrder) {
    label = CCLabelBMFont::create(placeholder.c_str(), font.c_str());
    parent->addChild(label, zOrder);
    label->setPosition(position);
    const float scale =
            0.6f * label->getContentSize().width > maxWidth ? maxWidth / label->getContentSize().width : 0.6f;
    label->setScale(scale);
    const auto bg = CCScale9Sprite::create("square02_small.png");
    parent->addChild(bg, zOrder);
    bg->setContentSize(bgSize);
    bg->setScale(0.5f);
    bg->setContentSize(bg->getContentSize() / 0.5f);
    bg->setPosition(position);
    bg->setOpacity(100);
    return bg;
}
void GDDLSearchLayer::createTextInputNode(CCLayer *parent, CCTextInputNode *&textfield, std::string font,
                                          std::string placeholder, CCPoint bgSize, CCPoint position,
                                          int zOrder) {
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
    const CCPoint positionLeft = object->getPosition() - CCPoint(object->getContentSize().width / 2 + size.x / 2, 0.0f);
    const auto leftButtonSprite = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
    const auto leftButton = CCMenuItemSpriteExtra::create(leftButtonSprite, this, leftCallback);
    object->getParent()->addChild(leftButton, zOrder);
    leftButton->setPosition(positionLeft);
    leftButton->setContentSize(size);
    // right
    const CCPoint positionRight =
            positionLeft + CCPoint(object->getContentSize().width + leftButton->getContentSize().width + 0.5f,
                                   0.0f); // why is this not symmetrical wtf
    const auto rightButtonSprite = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
    const auto rightButton = CCMenuItemSpriteExtra::create(rightButtonSprite, this, rightCallback);
    object->getParent()->addChild(rightButton, zOrder);
    rightButton->setPosition(positionRight);
    rightButton->setContentSize(size);
}

void GDDLSearchLayer::createCheckbox(CCLayer *parent, CCMenuItemToggler *&toggler, std::string label, float labelOffset, float scale,
                                     CCPoint position, SEL_MenuHandler callback, int zOrder) {
    toggler = CCMenuItemToggler::createWithStandardSprites(this, callback, scale);
    parent->addChild(toggler, zOrder);
    toggler->setPosition(position);
    const auto toggleLabel = CCLabelBMFont::create(label.c_str(), "bigFont.fnt");
    parent->addChild(toggleLabel, zOrder);
    toggleLabel->setPosition({toggler->getPositionX(), toggler->getPositionY()-labelOffset});
    toggleLabel->setScale(0.3f);
}

CCMenuItemSpriteExtra *GDDLSearchLayer::createTierNode(int tier) {
    // tier sprite
    const std::string tierString = tier != -1 ? std::to_string(tier) : "unrated";
    const std::string tierSpriteName = "tier_" + tierString + ".png";
    const auto textureName = Mod::get()->expandSpriteName(tierSpriteName.c_str());
    const auto tierSprite = CCSprite::create(textureName);
    tierSprite->setScale(0.15f);
    tierSprite->setContentSize({20.0f, 20.0f});
    const auto tierButton = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(GDDLSearchLayer::onTierSearch));
    tierButton->setContentSize({20.0f, 20.0f});
    tierButton->setID("button-tier-" + std::to_string(tier));
    return tierButton;
}

CCMenu *GDDLSearchLayer::createCheckboxNode(const std::string &idSuffix, const std::string &name) {
    const auto menu = CCMenu::create();
    menu->setLayout(RowLayout::create()->setGap(3.0f)->setAutoScale(true));
    // checkbox
    const auto toggler = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(GDDLSearchLayer::onSearchOptionSelected), 0.8f);
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

void GDDLSearchLayer::onToggleExactMatch(CCObject *sender) {}

void GDDLSearchLayer::onInGameRatingLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "You touched the left button", "OK")->show();
}

void GDDLSearchLayer::onInGameRatingRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "You touched the left button", "OK")->show();
}
void GDDLSearchLayer::onTierLowLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Low tier -", "OK")->show();
}
void GDDLSearchLayer::onTierLowRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Low tier +", "OK")->show();
}
void GDDLSearchLayer::onTierHighLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "High tier -", "OK")->show();
}

void GDDLSearchLayer::onTierHighRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "High tier +", "OK")->show();
}

void GDDLSearchLayer::onEnjoymentLowLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Low enj -", "OK")->show();
}

void GDDLSearchLayer::onEnjoymentLowRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Low enj +", "OK")->show();
}

void GDDLSearchLayer::onEnjoymentHighLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "High enj -", "OK")->show();
}

void GDDLSearchLayer::onEnjoymentHighRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "High enj +", "OK")->show();
}

void GDDLSearchLayer::onSubmissionCountLowLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Low sub count -", "OK")->show();
}

void GDDLSearchLayer::onSubmissionCountLowRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Low sub count +", "OK")->show();
}

void GDDLSearchLayer::onSubmissionCountHighLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "High sub count -", "OK")->show();
}

void GDDLSearchLayer::onSubmissionCountHighRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "High sub count +", "OK")->show();
}

void GDDLSearchLayer::onEnjSubmissionCountLowLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Low enj sub count -", "OK")->show();
}

void GDDLSearchLayer::onEnjSubmissionCountLowRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Low enj sub count +", "OK")->show();
}

void GDDLSearchLayer::onEnjSubmissionCountHighLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "High enj sub count -", "OK")->show();
}

void GDDLSearchLayer::onEnjSubmissionCountHighRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "High enj sub count +", "OK")->show();
}

void GDDLSearchLayer::onToggleNoUnrated(CCObject *sender) {
    FLAlertLayer::create("GDDL", "No unrated toggled", "OK")->show();
}

void GDDLSearchLayer::onToggleNoRated(CCObject *sender) {
    FLAlertLayer::create("GDDL", "No rated toggled", "OK")->show();
}

void GDDLSearchLayer::onToggleNoUnratedEnj(CCObject *sender) {
    FLAlertLayer::create("GDDL", "No unrated enj. toggled", "OK")->show();
}

void GDDLSearchLayer::onToggleNoRatedEnj(CCObject *sender) {
    FLAlertLayer::create("GDDL", "No rated enj. toggled", "OK")->show();
}

void GDDLSearchLayer::onSortByLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Sort by -", "OK")->show();
}

void GDDLSearchLayer::onSortByRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Sort by +", "OK")->show();
}

void GDDLSearchLayer::onSortDirectionLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Sort direction -", "OK")->show();
}

void GDDLSearchLayer::onSortDirectionRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "Sort direction +", "OK")->show();
}

GDDLSearchLayer *GDDLSearchLayer::create() {
    const auto newLayer = new GDDLSearchLayer();
    if(newLayer != nullptr && newLayer->init()) {
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

}

void GDDLSearchLayer::saveSettings() {

}
