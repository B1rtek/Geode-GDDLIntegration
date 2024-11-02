#include "GDDLRatingSubmissionLayer.h"
#include "Utils.h"

bool GDDLRatingSubmissionLayer::init(GJGameLevel* level) {
    if(!FLAlertLayer::init(75)) return false; // that magic number is actually bg opacity btw

    this->levelID = level->m_levelID;
    this->attempts = level->m_attempts;
    this->twoPlayer = level->m_twoPlayerMode;

    const CCPoint popupSize = {280.0f, 250.0f};
    const auto winSize = CCDirector::sharedDirector()->getWinSize();

    // background
    const auto bg = CCScale9Sprite::create("GJ_square05.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });
    bg->setID("gddl-rating-submit-bg"_spr);
    m_mainLayer->addChild(bg, -1);

    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("gddl-rating-submit-menu"_spr);
    m_buttonMenu->setContentSize(popupSize);
    m_buttonMenu->setPosition({ winSize.width / 2 - popupSize.x / 2, winSize.height / 2 - popupSize.y / 2});
    m_mainLayer->addChild(m_buttonMenu, 10);
    // title
    const auto title = CCLabelBMFont::create("Submit rating", "goldFont.fnt");
    title->setID("gddl-rating-submit-title"_spr);
    title->setPosition({popupSize.x / 2, popupSize.y - 20.0f});
    m_buttonMenu->addChild(title);
    // close button
    const auto closeButtonSprite = CircleButtonSprite::createWithSpriteFrameName("geode.loader/close.png", .85f,CircleBaseColor::Gray);
    m_closeBtn = CCMenuItemSpriteExtra::create(closeButtonSprite, this, menu_selector(GDDLRatingSubmissionLayer::onClose));
    m_buttonMenu->addChild(m_closeBtn);
    m_closeBtn->setPosition({3.0f, popupSize.y - 3.0f});

    // content
    // rating
    Utils::createTextInputNode(m_buttonMenu, ratingTextfield, "bigFont.fnt", "", 2, {35.0f, 25.0f}, {popupSize.x / 4, popupSize.y - 50.0f});
    ratingTextfield->setAllowedChars("1234567890");
    Utils::createLeftRightButtonsAround(ratingTextfield, {13.0f, 19.0f}, this, menu_selector(GDDLRatingSubmissionLayer::onRatingLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onRatingRight));
    // enjoyment
    Utils::createTextInputNode(m_buttonMenu, enjoymentTextfield, "bigFont.fnt", "", 2, {35.0f, 25.0f}, {3 * popupSize.x / 4, popupSize.y - 50.0f});
    enjoymentTextfield->setAllowedChars("1234567890");
    Utils::createLeftRightButtonsAround(enjoymentTextfield, {13.0f, 19.0f}, this, menu_selector(GDDLRatingSubmissionLayer::onEnjoymentLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onEnjoymentRight));
    // fps value
    Utils::createTextInputNode(m_buttonMenu, fpsTextfield, "bigFont.fnt", "", 4, {35.0f, 25.0f}, {popupSize.x / 4, popupSize.y - 80.0f});
    fpsTextfield->setAllowedChars("1234567890");
    // device
    const auto choiceLabelBG = Utils::createLabelForChoice(m_buttonMenu, deviceLabel, "bigFont.fnt", "Any", 80.0f, {3 * popupSize.x / 4, popupSize.y - 80.0f},
                                          {80.0f, 25.0f});
    Utils::createLeftRightButtonsAround(choiceLabelBG, {13.0f, 19.0f}, this, menu_selector(GDDLRatingSubmissionLayer::onDeviceLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onDeviceRight));
    // proof
    Utils::createTextInputNode(m_buttonMenu, proofTextfield, "chatFont.fnt", "", 256, {170.0f, 25.0f}, {popupSize.x / 2, popupSize.y - 110.0f});
    proofTextfield->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
    // percent
    Utils::createTextInputNode(m_buttonMenu, percentTextfield, "bigFont.fnt", "", 3, {35.0f, 25.0f}, {popupSize.x / 4, popupSize.y - 140.0f});
    percentTextfield->setAllowedChars("1234567890");
    // attempts
    Utils::createTextInputNode(m_buttonMenu, attemptsTextfield, "bigFont.fnt", "", 10, {35.0f, 25.0f}, {3 * popupSize.x / 4, popupSize.y - 140.0f});
    percentTextfield->setAllowedChars("1234567890");
    if (twoPlayer) {
        Utils::createCheckbox(m_buttonMenu, soloCompletionToggler, "Solo completion", 17.5f, 0.9f, {popupSize.x / 4, popupSize.y - 170.0f}, this,
                              menu_selector(GDDLRatingSubmissionLayer::onToggleSoloCompletion));
        Utils::createTextInputNode(m_buttonMenu, secondPlayerTextfield, "bigFont.fnt", "", 32, {80.0f, 25.0f}, {3 * popupSize.x / 4, popupSize.y - 170.0f});
        secondPlayerTextfield->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
    }
    // submit button
    const auto submitButtonSprite = ButtonSprite::create("Submit", "bigFont.fnt", "GJ_button_01.png");
    submitButtonSprite->setScale(0.6f);
    const auto submitButton = CCMenuItemSpriteExtra::create(submitButtonSprite, this, menu_selector(GDDLRatingSubmissionLayer::onSubmitClicked));
    submitButton->setID("gddl-rating-submit-submit-button"_spr);
    submitButton->setPosition({popupSize.x / 2, popupSize.y - 200.0f});
    m_buttonMenu->addChild(submitButton);
    return true;
}

void GDDLRatingSubmissionLayer::onClose(CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLRatingSubmissionLayer::onRatingLeft(CCObject *sender) {

}

void GDDLRatingSubmissionLayer::onRatingRight(CCObject *sender) {

}

void GDDLRatingSubmissionLayer::onEnjoymentLeft(CCObject *sender) {

}

void GDDLRatingSubmissionLayer::onEnjoymentRight(CCObject *sender) {

}

void GDDLRatingSubmissionLayer::onDeviceRight(CCObject *sender) {

}

void GDDLRatingSubmissionLayer::onDeviceLeft(CCObject *sender) {

}

void GDDLRatingSubmissionLayer::onToggleSoloCompletion(CCObject *sender) {

}

void GDDLRatingSubmissionLayer::onSubmitClicked(CCObject *sender) {

}

GDDLRatingSubmissionLayer *GDDLRatingSubmissionLayer::create(GJGameLevel* level) {
    if (const auto newLayer = new GDDLRatingSubmissionLayer(); newLayer != nullptr && newLayer->init(level)) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLRatingSubmissionLayer::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}
