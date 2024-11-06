#include "GDDLRatingSubmissionLayer.h"
#include "Utils.h"

bool GDDLRatingSubmissionLayer::init(GJGameLevel *level) {
    if (!FLAlertLayer::init(75)) return false; // that magic number is actually bg opacity btw

    this->levelID = level->m_levelID;
    this->percent = level->m_normalPercent;
    this->attempts = level->m_attempts;
    this->twoPlayer = level->m_twoPlayerMode;
    setInitialValues();

    const CCPoint popupSize = {220.0f, this->twoPlayer ? 280.0f : 240.0f};
    const auto winSize = CCDirector::sharedDirector()->getWinSize();

    // background
    const auto bg = CCScale9Sprite::create("GJ_square05.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({winSize.width / 2, winSize.height / 2});
    bg->setID("gddl-rating-submit-bg"_spr);
    m_mainLayer->addChild(bg, -1);

    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("gddl-rating-submit-menu"_spr);
    m_buttonMenu->setContentSize(popupSize);
    m_buttonMenu->setPosition({winSize.width / 2 - popupSize.x / 2, winSize.height / 2 - popupSize.y / 2});
    m_mainLayer->addChild(m_buttonMenu, 10);
    // title
    const auto title = CCLabelBMFont::create("Submit rating", "goldFont.fnt");
    title->setID("gddl-rating-submit-title"_spr);
    title->setPosition({popupSize.x / 2, popupSize.y - 20.0f});
    m_buttonMenu->addChild(title);
    // close button
    const auto closeButtonSprite = CircleButtonSprite::createWithSpriteFrameName("geode.loader/close.png", .85f,
                                                                                 CircleBaseColor::Gray);
    m_closeBtn = CCMenuItemSpriteExtra::create(closeButtonSprite, this,
                                               menu_selector(GDDLRatingSubmissionLayer::onClose));
    m_buttonMenu->addChild(m_closeBtn);
    m_closeBtn->setPosition({3.0f, popupSize.y - 3.0f});

    // content
    const auto infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoButtonSprite->setScale(0.5f);
    // rating
    const auto ratingLabel = addLabel("Tier", {popupSize.x / 4, popupSize.y - 40.0f});
    Utils::createTextInputNode(m_buttonMenu, ratingTextfield, "bigFont.fnt", "-", 2, {35.0f, 25.0f},
                               {popupSize.x / 4, popupSize.y - 60.0f});
    ratingTextfield->setAllowedChars("-1234567890");
    Utils::createLeftRightButtonsAround(ratingTextfield, {13.0f, 19.0f}, this,
                                        menu_selector(GDDLRatingSubmissionLayer::onRatingLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onRatingRight));
    addInfoButton(ratingLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onRatingInfo));
    // enjoyment
    const auto enjoymentLabel = addLabel("Enjoyment", {3 * popupSize.x / 4, popupSize.y - 40.0f});
    Utils::createTextInputNode(m_buttonMenu, enjoymentTextfield, "bigFont.fnt", "-", 2, {35.0f, 25.0f},
                               {3 * popupSize.x / 4, popupSize.y - 60.0f});
    enjoymentTextfield->setAllowedChars("-1234567890");
    Utils::createLeftRightButtonsAround(enjoymentTextfield, {13.0f, 19.0f}, this,
                                        menu_selector(GDDLRatingSubmissionLayer::onEnjoymentLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onEnjoymentRight));
    addInfoButton(enjoymentLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onEnjoymentInfo));
    // fps value
    const auto fpsLabel = addLabel("FPS", {popupSize.x / 4, popupSize.y - 80.0f});
    Utils::createTextInputNode(m_buttonMenu, fpsTextfield, "bigFont.fnt", "", 4, {35.0f, 25.0f},
                               {popupSize.x / 4, popupSize.y - 100.0f});
    fpsTextfield->setAllowedChars("1234567890");
    Utils::createLeftRightButtonsAround(fpsTextfield, {13.0f, 19.0f}, this,
                                        menu_selector(GDDLRatingSubmissionLayer::onFPSLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onFPSRight));
    addInfoButton(fpsLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onFPSInfo));
    // device
    addLabel("Device", {3 * popupSize.x / 4 - 22.5f, popupSize.y - 80.0f});
    const auto choiceLabelBG = Utils::createLabelForChoice(m_buttonMenu, deviceLabel, "bigFont.fnt",
                                                           device[mobile ? 1 : 0].c_str(), 80.0f,
                                                           {3 * popupSize.x / 4 - 22.5f, popupSize.y - 100.0f},
                                                           {80.0f, 25.0f});
    Utils::createLeftRightButtonsAround(choiceLabelBG, {13.0f, 19.0f}, this,
                                        menu_selector(GDDLRatingSubmissionLayer::onDeviceLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onDeviceRight));
    // percent
    const auto percentLabel = addLabel("Percent", {popupSize.x / 4, popupSize.y - 120.0f});
    Utils::createTextInputNode(m_buttonMenu, percentTextfield, "bigFont.fnt", "", 3, {35.0f, 25.0f},
                               {popupSize.x / 4, popupSize.y - 140.0f});
    percentTextfield->setAllowedChars("1234567890");
    addLabel("%", {popupSize.x / 4 + 30.0f, popupSize.y - 140.0f}, 0.7f, "bigFont.fnt");
    addInfoButton(percentLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onPercentInfo));
    // attempts
    const auto attemptsLabel = addLabel("Attempts", {3 * popupSize.x / 4 - 7.5f, popupSize.y - 120.0f});
    Utils::createTextInputNode(m_buttonMenu, attemptsTextfield, "bigFont.fnt", "", 9, {50.0f, 25.0f},
                               {3 * popupSize.x / 4 - 7.5f, popupSize.y - 140.0f});
    percentTextfield->setAllowedChars("1234567890");
    addInfoButton(attemptsLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onAttemptsInfo));
    // proof
    const auto proofLabel = addLabel("Proof", {popupSize.x / 2, popupSize.y - 160.0f});
    Utils::createTextInputNode(m_buttonMenu, proofTextfield, "chatFont.fnt", "", 256, {200.0f, 25.0f},
                               {popupSize.x / 2, popupSize.y - 180.0f});
    proofTextfield->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
    addInfoButton(proofLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onProofInfo));
    if (twoPlayer) {
        Utils::createCheckbox(m_buttonMenu, soloCompletionToggler, "Solo completion", 17.5f, 0.9f,
                              {popupSize.x / 4, popupSize.y - 220.0f}, this,
                              menu_selector(GDDLRatingSubmissionLayer::onToggleSoloCompletion));
        soloCompletionToggler->toggle(soloCompletion);
        const auto secondPlayerLabel = addLabel("Second player", {3 * popupSize.x / 4 - 35.0f, popupSize.y - 200.0f});
        Utils::createTextInputNode(m_buttonMenu, secondPlayerTextfield, "bigFont.fnt", "", 32, {110.0f, 25.0f},
                                   {3 * popupSize.x / 4 - 35.0f, popupSize.y - 220.0f});
        secondPlayerTextfield->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
        addInfoButton(secondPlayerLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onSecondPlayerInfo));
    }
    // submit button
    const auto submitButtonSprite = ButtonSprite::create("Submit", "bigFont.fnt", "GJ_button_01.png");
    submitButtonSprite->setScale(0.6f);
    const auto submitButton = CCMenuItemSpriteExtra::create(submitButtonSprite, this,
                                                            menu_selector(GDDLRatingSubmissionLayer::onSubmitClicked));
    submitButton->setID("gddl-rating-submit-submit-button"_spr);
    submitButton->setPosition({popupSize.x / 2, 22.0f});
    m_buttonMenu->addChild(submitButton);

    updateTextfields();

    return true;
}

void GDDLRatingSubmissionLayer::onClose(CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLRatingSubmissionLayer::onRatingLeft(CCObject *sender) {
    rating = Utils::getNumberTextfieldValue(ratingTextfield) - 1;
    if (rating < 0) {
        rating = ExcludeRangeSettingV3::highestTier;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(rating, ratingTextfield, 0, "-");
}

void GDDLRatingSubmissionLayer::onRatingRight(CCObject *sender) {
    rating = Utils::getNumberTextfieldValue(ratingTextfield) + 1;
    if (rating > ExcludeRangeSettingV3::highestTier) {
        rating = 0;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(rating, ratingTextfield, 0, "-");
}

void GDDLRatingSubmissionLayer::onEnjoymentLeft(CCObject *sender) {
    enjoyment = Utils::getNumberWithGivenDefaultTextfieldValue(enjoymentTextfield, -1) - 1;
    if (enjoyment < -1) {
        enjoyment = 10;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(enjoyment, enjoymentTextfield, -1, "-");
}

void GDDLRatingSubmissionLayer::onEnjoymentRight(CCObject *sender) {
    enjoyment = Utils::getNumberWithGivenDefaultTextfieldValue(enjoymentTextfield, -1) + 1;
    if (enjoyment > 10) {
        enjoyment = -1;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(enjoyment, enjoymentTextfield, -1, "-");
}

void GDDLRatingSubmissionLayer::onFPSLeft(CCObject *sender) {
    fps = Utils::getNumberWithGivenDefaultTextfieldValue(fpsTextfield, -1) - 1;
    if (fps < 30) {
        fps = 30;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(fps, fpsTextfield, -1);
}

void GDDLRatingSubmissionLayer::onFPSRight(CCObject *sender) {
    fps = Utils::getNumberWithGivenDefaultTextfieldValue(fpsTextfield, -1) + 1;
    if (fps > 9999) {
        fps = 9999;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(fps, fpsTextfield, -1);
}

void GDDLRatingSubmissionLayer::onDeviceRight(CCObject *sender) {
    mobile = !mobile;
    deviceLabel->setString(device[mobile ? 1 : 0].c_str());
    Utils::scaleLabelToWidth(deviceLabel, 80.0f);
}

void GDDLRatingSubmissionLayer::onDeviceLeft(CCObject *sender) {
    onDeviceRight(sender);
}

void GDDLRatingSubmissionLayer::onToggleSoloCompletion(CCObject *sender) {
    soloCompletion = !soloCompletionToggler->isOn();
}

void GDDLRatingSubmissionLayer::onSubmitClicked(CCObject *sender) {
    FLAlertLayer::create("submit", "submit clicked", "yay")->show();
}

void GDDLRatingSubmissionLayer::onRatingInfo(CCObject *sender) {
    FLAlertLayer::create("Rating", "Must be from <cr>1</c> to <cr>" + std::to_string(ExcludeRangeSettingV3::highestTier) + "</c>, enter '-' if you don't want to submit the tier rating", "OK")->show();
}

void GDDLRatingSubmissionLayer::onEnjoymentInfo(CCObject *sender) {
    FLAlertLayer::create("Enjoyment", "From <cr>0</c> (abysmal) to <cg>10</c> (masterpiece), enter '-' if you don't want to submit the enjoyment rating", "OK")->show();
}

void GDDLRatingSubmissionLayer::onFPSInfo(CCObject *sender) {
    FLAlertLayer::create("FPS", "At least <cg>30</c>", "OK")->show();
}

void GDDLRatingSubmissionLayer::onPercentInfo(CCObject *sender) {
    FLAlertLayer::create("Percent", "Autofilled according to <cb>your current progress</c>, will not affect ratings or get sent to the queue if <cr>under 100</c>, defaults to 100", "OK")->show();
}

void GDDLRatingSubmissionLayer::onAttemptsInfo(CCObject *sender) {
    FLAlertLayer::create("Percent", "Autofilled according to <cb>your current progress</c>, optional - if you don't want to share it, just <cr>remove it</c>", "OK")->show();
}

void GDDLRatingSubmissionLayer::onProofInfo(CCObject *sender) {
    FLAlertLayer::create("Proof", "Proof is <cy>required</c> for <cr>extreme demons</c>. Clicks <cy>must be included</c> if the level is <cr>tier 31 or higher</c>, proof <co>must contain the endscreen</c>. Accepted sites include: <cb>Youtube, Twitch, Google drive and BiliBili</c>. Submitting <cy>joke proof</c> will result in a <cr>temporary ban</c>.", "OK")->show();
}

void GDDLRatingSubmissionLayer::onSecondPlayerInfo(CCObject *sender) {
    FLAlertLayer::create("Second player", "Fill out with the username of the <cy>person you completed the level with</c> if you <co>didn't complete it solo</c>, if they don't have a <cr>GDDL account</c> leave this blank", "OK")->show();
}

CCLabelBMFont *
GDDLRatingSubmissionLayer::addLabel(const std::string &text, const CCPoint &position, float scale, std::string font) {
    const auto label = CCLabelBMFont::create(text.c_str(), font.c_str());
    label->setScale(scale);
    label->setPosition(position);
    m_buttonMenu->addChild(label);
    return label;
}

void GDDLRatingSubmissionLayer::addInfoButton(CCLabelBMFont *label, CCSprite *iButtonSprite, SEL_MenuHandler callback) {
    const auto ratingInfoButton = CCMenuItemSpriteExtra::create(iButtonSprite, this,
                                                                callback);
    ratingInfoButton->setPosition({label->getPositionX() + label->getScaledContentWidth() / 2 + 10.0f, label->getPositionY()});
    m_buttonMenu->addChild(ratingInfoButton);
}

void GDDLRatingSubmissionLayer::setInitialValues() {
    const auto gddlRating = RatingsManager::getRating(levelID);
    rating = gddlRating ? gddlRating.value().rating : -1;
    enjoyment = gddlRating ? static_cast<int>(std::round(gddlRating.value().enjoyment)) : -1;
    fps = Utils::getCorrectedFPS();
    mobile = Utils::isMobile();
}

void GDDLRatingSubmissionLayer::updateTextfields() {
    Utils::setNumberWithGivenDefaultValueTextfield(rating, ratingTextfield, 0, "-");
    Utils::setNumberWithGivenDefaultValueTextfield(enjoyment, enjoymentTextfield, -1, "-");
    Utils::setNumberWithGivenDefaultValueTextfield(fps, fpsTextfield, -1);
    percentTextfield->setString(std::to_string(this->percent));
    attemptsTextfield->setString(std::to_string(this->attempts));
}

GDDLRatingSubmissionLayer *GDDLRatingSubmissionLayer::create(GJGameLevel *level) {
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

