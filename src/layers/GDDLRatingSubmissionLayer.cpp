#include "GDDLRatingSubmissionLayer.h"
#include "Utils.h"
#include "settings/LoginSettingNodeV3.h"
#include "GDDLLoginLayer.h"

bool GDDLRatingSubmissionLayer::init(GJGameLevel* level, int gddlLevelID) {
    if (!FLAlertLayer::init(75)) return false; // that magic number is actually bg opacity btw

    this->gddlLevelID = gddlLevelID;
    this->percent = level->m_normalPercent;
    this->attempts = level->m_attempts;
    this->twoPlayer = level->m_twoPlayerMode;
    setInitialValues();

    const CCPoint popupSize = {220.0f, this->twoPlayer ? 300.0f : 250.0f};
    const auto winSize = CCDirector::sharedDirector()->getWinSize();

    // background
    const auto bg = CCScale9Sprite::create(Utils::getGrayPopupBG().c_str(), {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({winSize.width / 2, winSize.height / 2 - 9.0f});
    bg->setID("gddl-rating-submit-bg"_spr);
    m_mainLayer->addChild(bg, -1);

    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("gddl-rating-submit-menu"_spr);
    m_buttonMenu->setContentSize(popupSize);
    m_buttonMenu->setPosition({winSize.width / 2 - popupSize.x / 2, winSize.height / 2 - popupSize.y / 2 - 9.0f});
    m_mainLayer->addChild(m_buttonMenu, 10);
    // title
    const auto title = CCLabelBMFont::create("Submit rating", "goldFont.fnt");
    title->setID("gddl-rating-submit-title"_spr);
    title->setScale(0.9f);
    title->setPosition({popupSize.x / 2, popupSize.y - 20.0f});
    m_buttonMenu->addChild(title);
    // close button
    const auto closeButtonSprite = Utils::getGrayPopupCloseButton();
    m_closeBtn = CCMenuItemSpriteExtra::create(closeButtonSprite, this,
                                               menu_selector(GDDLRatingSubmissionLayer::onClose));
    m_buttonMenu->addChild(m_closeBtn);
    m_closeBtn->setPosition({3.0f, popupSize.y - 3.0f});

    // content
    const auto infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoButtonSprite->setScale(0.5f);
    // rating
    const auto ratingLabel = addLabel("Tier", {popupSize.x / 4 + 7.5f, popupSize.y - 40.0f});
    Utils::createTextInputNode(m_buttonMenu, ratingTextfield, "bigFont.fnt", "-", 2, {35.0f, 25.0f},
                               {popupSize.x / 4 + 7.5f, popupSize.y - 60.0f});
    ratingTextfield->setAllowedChars("-1234567890");
    Utils::createLeftRightButtonsAround(ratingTextfield, {13.0f, 19.0f}, this,
                                        menu_selector(GDDLRatingSubmissionLayer::onRatingLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onRatingRight));
    addInfoButtonAndCenterLabel(ratingLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onRatingInfo), popupSize.x / 4 + 7.5f);
    // enjoyment
    const auto enjoymentLabel = addLabel("Enjoyment", {3 * popupSize.x / 4 - 7.5f, popupSize.y - 40.0f});
    Utils::createTextInputNode(m_buttonMenu, enjoymentTextfield, "bigFont.fnt", "-", 2, {35.0f, 25.0f},
                               {3 * popupSize.x / 4 - 7.5f, popupSize.y - 60.0f});
    enjoymentTextfield->setAllowedChars("-1234567890");
    Utils::createLeftRightButtonsAround(enjoymentTextfield, {13.0f, 19.0f}, this,
                                        menu_selector(GDDLRatingSubmissionLayer::onEnjoymentLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onEnjoymentRight));
    addInfoButtonAndCenterLabel(enjoymentLabel, infoButtonSprite,
                                menu_selector(GDDLRatingSubmissionLayer::onEnjoymentInfo), 3 * popupSize.x / 4 - 7.5f);
    // fps value
    const auto fpsLabel = addLabel("FPS", {popupSize.x / 4 + 7.5f, popupSize.y - 85.0f});
    Utils::createTextInputNode(m_buttonMenu, fpsTextfield, "bigFont.fnt", "", 4, {35.0f, 25.0f},
                               {popupSize.x / 4 + 7.5f, popupSize.y - 105.0f});
    fpsTextfield->setAllowedChars("1234567890");
    Utils::createLeftRightButtonsAround(fpsTextfield, {13.0f, 19.0f}, this,
                                        menu_selector(GDDLRatingSubmissionLayer::onFPSLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onFPSRight));
    addInfoButtonAndCenterLabel(fpsLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onFPSInfo), popupSize.x / 4 + 7.5f);
    // device
    addLabel("Device", {3 * popupSize.x / 4 - 7.5f, popupSize.y - 85.0f});
    const auto choiceLabelBG = Utils::createLabelForChoice(m_buttonMenu, deviceLabel, "bigFont.fnt",
                                                           device[mobile ? 1 : 0].c_str(), 50.0f,
                                                           {3 * popupSize.x / 4 - 7.5f, popupSize.y - 105.0f},
                                                           {50.0f, 25.0f});
    Utils::createLeftRightButtonsAround(choiceLabelBG, {13.0f, 19.0f}, this,
                                        menu_selector(GDDLRatingSubmissionLayer::onDeviceLeft),
                                        menu_selector(GDDLRatingSubmissionLayer::onDeviceRight));
    // percent
    const auto percentLabel = addLabel("Percent", {popupSize.x / 4 + 7.5f, popupSize.y - 130.0f});
    Utils::createTextInputNode(m_buttonMenu, percentTextfield, "bigFont.fnt", "", 3, {35.0f, 25.0f},
                               {popupSize.x / 4 + 7.5f, popupSize.y - 150.0f});
    percentTextfield->setAllowedChars("1234567890");
    addLabel("%", {popupSize.x / 4 + 37.5f, popupSize.y - 150.0f}, 0.7f, "bigFont.fnt");
    addInfoButtonAndCenterLabel(percentLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onPercentInfo), popupSize.x / 4 + 7.5f);
    // attempts
    const auto attemptsLabel = addLabel("Attempts", {3 * popupSize.x / 4 - 7.5f, popupSize.y - 130.0f});
    Utils::createTextInputNode(m_buttonMenu, attemptsTextfield, "bigFont.fnt", "", 9, {50.0f, 25.0f},
                               {3 * popupSize.x / 4 - 7.5f, popupSize.y - 150.0f});
    percentTextfield->setAllowedChars("1234567890");
    addInfoButtonAndCenterLabel(attemptsLabel, infoButtonSprite,
                                menu_selector(GDDLRatingSubmissionLayer::onAttemptsInfo), 3 * popupSize.x / 4 - 7.5f);
    // proof
    const auto proofLabel = addLabel("Proof", {popupSize.x / 2, popupSize.y - 175.0f});
    Utils::createTextInputNode(m_buttonMenu, proofTextfield, "chatFont.fnt", "", 256, {200.0f, 25.0f},
                               {popupSize.x / 2, popupSize.y - 195.0f});
    proofTextfield->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
    addInfoButtonAndCenterLabel(proofLabel, infoButtonSprite, menu_selector(GDDLRatingSubmissionLayer::onProofInfo), popupSize.x / 2);
    if (twoPlayer) {
        addLabel("Solo completion", {popupSize.x / 4, popupSize.y - 220.0f}, 0.6f);
        soloCompletionToggler = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(GDDLRatingSubmissionLayer::onToggleSoloCompletion), 0.9f);
        soloCompletionToggler->setPosition({popupSize.x / 4, popupSize.y - 240.0f});
        soloCompletionToggler->toggle(soloCompletion);
        m_buttonMenu->addChild(soloCompletionToggler);
        const auto secondPlayerLabel = addLabel("Second player", {3 * popupSize.x / 4 - 35.0f, popupSize.y - 220.0f});
        Utils::createTextInputNode(m_buttonMenu, secondPlayerTextfield, "bigFont.fnt", "", 32, {110.0f, 25.0f},
                                   {3 * popupSize.x / 4 - 35.0f, popupSize.y - 240.0f});
        secondPlayerTextfield->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
        addInfoButtonAndCenterLabel(secondPlayerLabel, infoButtonSprite,
                                    menu_selector(GDDLRatingSubmissionLayer::onSecondPlayerInfo), 3 * popupSize.x / 4 - 35.0f);
    }
    // submit button
    const auto submitButtonSprite = ButtonSprite::create("Submit", "bigFont.fnt", "GJ_button_01.png");
    submitButtonSprite->setScale(0.5f);
    const auto submitButton = CCMenuItemSpriteExtra::create(submitButtonSprite, this,
                                                            menu_selector(GDDLRatingSubmissionLayer::onSubmitClicked));
    submitButton->setID("gddl-rating-submit-submit-button"_spr);
    submitButton->setPosition({popupSize.x - 53.0f, 22.0f});
    m_buttonMenu->addChild(submitButton);
    // guidelines button
    const auto guidelinesButtonSprite = ButtonSprite::create("Guidelines", "bigFont.fnt", "GJ_button_02.png");
    guidelinesButtonSprite->setScale(0.5f);
    const auto guidelinesButton = CCMenuItemSpriteExtra::create(guidelinesButtonSprite, this,
                                                                menu_selector(
                                                                    GDDLRatingSubmissionLayer::onGuidelinesClicked));
    guidelinesButton->setPosition({69.0f, 22.0f});
    m_buttonMenu->addChild(guidelinesButton);

    updateTextfields();
    prepareSubmissionListeners();

    return true;
}

void GDDLRatingSubmissionLayer::onClose(CCObject* sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLRatingSubmissionLayer::onRatingLeft(CCObject* sender) {
    rating = Utils::getNumberTextfieldValue(ratingTextfield) - 1;
    if (rating < 0) {
        rating = ExcludeRangeSettingV3::highestTier;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(rating, ratingTextfield, 0, "-");
}

void GDDLRatingSubmissionLayer::onRatingRight(CCObject* sender) {
    rating = Utils::getNumberTextfieldValue(ratingTextfield) + 1;
    if (rating > ExcludeRangeSettingV3::highestTier) {
        rating = 0;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(rating, ratingTextfield, 0, "-");
}

void GDDLRatingSubmissionLayer::onEnjoymentLeft(CCObject* sender) {
    enjoyment = Utils::getNumberWithGivenDefaultTextfieldValue(enjoymentTextfield, -1) - 1;
    if (enjoyment < -1) {
        enjoyment = 10;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(enjoyment, enjoymentTextfield, -1, "-");
}

void GDDLRatingSubmissionLayer::onEnjoymentRight(CCObject* sender) {
    enjoyment = Utils::getNumberWithGivenDefaultTextfieldValue(enjoymentTextfield, -1) + 1;
    if (enjoyment > 10) {
        enjoyment = -1;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(enjoyment, enjoymentTextfield, -1, "-");
}

void GDDLRatingSubmissionLayer::onFPSLeft(CCObject* sender) {
    fps = Utils::getNumberWithGivenDefaultTextfieldValue(fpsTextfield, -1) - 1;
    if (fps < 30) {
        fps = 30;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(fps, fpsTextfield, -1);
}

void GDDLRatingSubmissionLayer::onFPSRight(CCObject* sender) {
    fps = Utils::getNumberWithGivenDefaultTextfieldValue(fpsTextfield, -1) + 1;
    if (fps > 9999) {
        fps = 9999;
    }
    Utils::setNumberWithGivenDefaultValueTextfield(fps, fpsTextfield, -1);
}

void GDDLRatingSubmissionLayer::onDeviceRight(CCObject* sender) {
    mobile = !mobile;
    deviceLabel->setString(device[mobile ? 1 : 0].c_str());
    Utils::scaleLabelToWidth(deviceLabel, 50.0f);
}

void GDDLRatingSubmissionLayer::onDeviceLeft(CCObject* sender) {
    onDeviceRight(sender);
}

void GDDLRatingSubmissionLayer::onToggleSoloCompletion(CCObject* sender) {
    soloCompletion = !soloCompletionToggler->isOn();
}

/**
 * json schema
 * {
 *  "levelID":341613,
 *  "rating":2,
 *  "enjoyment":6,
 *  "refreshRate":60,
 *  "device": "pc" or "mobile" - api V2 change
 *  "proof":"https://youtu.be/3-BUEoH9WBs",
 *  "progress":100,
 *  "attempts":403,
 *  "isSolo":true - default for both 1p and 2p levels
 *  "secondPlayerID": int | null
 *  }
 *
 *  headers:
 *  Cookie: gddl.sid.sig=<sid.sig>; gddl.sid=<sid>
 */

void GDDLRatingSubmissionLayer::onSubmitClicked(CCObject* sender) {
    submissionJson = matjson::Value();
    if (const std::string error = fillOutSubmissionJson(); !error.empty()) {
        Notification::create(error, NotificationIcon::Warning, 2)->show();
        return;
    }
    // fill out 2p stuff
    if (this->twoPlayer) {
        submissionJson["isSolo"] = soloCompletion;
        if (!soloCompletion) {
            // a request to retrieve the userid has to be made first before making the submission request
            std::string requestURL = userSearchEndpoint;
            requestedUsername = secondPlayerTextfield->getString();
            requestURL += "?name=" + requestedUsername + (time(nullptr) < Utils::API_SWITCH_TIME ? "&chunk=25" : "&limit=25");
            auto req = web::WebRequest();
            req.header("User-Agent", Utils::getUserAgent());
            userSearchListener.setFilter(req.get(requestURL));
        } else {
            makeSubmissionRequest();
        }
    } else {
        makeSubmissionRequest();
    }
}

void GDDLRatingSubmissionLayer::onGuidelinesClicked(CCObject* sender) {
    web::openLinkInBrowser("https://gdladder.com/about#guidelines");
}

void GDDLRatingSubmissionLayer::onRatingInfo(CCObject* sender) {
    FLAlertLayer::create(
        "Rating",
        "Must be from <cr>1</c> to <cr>" + std::to_string(ExcludeRangeSettingV3::highestTier) +
        "</c>, enter '-' if you don't want to submit the tier rating", "OK")->show();
}

void GDDLRatingSubmissionLayer::onEnjoymentInfo(CCObject* sender) {
    FLAlertLayer::create("Enjoyment",
                         "From <cr>0</c> (abysmal) to <cg>10</c> (masterpiece), enter '-' if you don't want to submit the enjoyment rating",
                         "OK")->show();
}

void GDDLRatingSubmissionLayer::onFPSInfo(CCObject* sender) {
    FLAlertLayer::create("FPS", "At least <cg>30</c>", "OK")->show();
}

void GDDLRatingSubmissionLayer::onPercentInfo(CCObject* sender) {
    FLAlertLayer::create(
        "Percent",
        "Autofilled according to <cb>your current progress</c>, will not affect ratings or get sent to the queue if <cr>under 100</c>, defaults to 100",
        "OK")->show();
}

void GDDLRatingSubmissionLayer::onAttemptsInfo(CCObject* sender) {
    FLAlertLayer::create(
        "Percent",
        "Autofilled according to <cb>your current progress</c>, optional - if you don't want to share it, just <cr>remove it</c>",
        "OK")->show();
}

void GDDLRatingSubmissionLayer::onProofInfo(CCObject* sender) {
    FLAlertLayer::create(
        "Proof",
        "Proof is <cy>required</c> for <cr>extreme demons</c>. Clicks <cy>must be included</c> if the level is <cr>tier 31 or higher</c>, proof <co>must contain the endscreen</c>. Accepted sites include: <cb>Youtube, Twitch, Google drive and BiliBili</c>. Submitting <cy>joke proof</c> will result in a <cr>temporary ban</c>.",
        "OK")->show();
}

void GDDLRatingSubmissionLayer::onSecondPlayerInfo(CCObject* sender) {
    FLAlertLayer::create("Second player",
                         "Fill out with the username of the <cy>person you completed the level with</c> if you <co>didn't complete it solo</c>, if they don't have a <cr>GDDL account</c> leave this blank",
                         "OK")->show();
}

CCLabelBMFont*
GDDLRatingSubmissionLayer::addLabel(const std::string& text, const CCPoint& position, float scale, std::string font) {
    const auto label = CCLabelBMFont::create(text.c_str(), font.c_str());
    label->setScale(scale);
    label->setPosition(position);
    m_buttonMenu->addChild(label);
    return label;
}

void GDDLRatingSubmissionLayer::addInfoButtonAndCenterLabel(CCLabelBMFont* label, CCSprite* iButtonSprite, SEL_MenuHandler callback, const float centerAroundX) {
    const auto ratingInfoButton = CCMenuItemSpriteExtra::create(iButtonSprite, this,
                                                                callback);
    const float gap = 10.0f;
    const float totalLength = label->getScaledContentWidth() + gap + ratingInfoButton->getScaledContentWidth() / 2;
    label->setPosition({centerAroundX - totalLength / 2 + label->getScaledContentWidth() / 2, label->getPositionY()});
    ratingInfoButton->setPosition({
        label->getPositionX() + label->getScaledContentWidth() / 2 + 10.0f, label->getPositionY()
    });
    m_buttonMenu->addChild(ratingInfoButton);
}

void GDDLRatingSubmissionLayer::setInitialValues() {
    const auto gddlRating = RatingsManager::getRating(this->gddlLevelID);
    rating = gddlRating ? gddlRating.value().roundedRating : -1;
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

void GDDLRatingSubmissionLayer::prepareSubmissionListeners() {
    submissionListener.bind([this](web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            if (res->code() == 200) {
                std::string message = "Rating submitted!";
                if (jsonResponse.contains("wasAuto") && jsonResponse["wasAuto"].isBool() && jsonResponse["wasAuto"].asBool().unwrap()) {
                    message = "Submission accepted!";
                }
                // cache submitted submission
                Submission submitted = Submission(submissionJson, true);
                RatingsManager::cacheSubmission(this->gddlLevelID, submitted);
                Notification::create(message, NotificationIcon::Success, 2)->show();
                onClose(nullptr);
            } else {
                const std::string error = jsonResponse["message"].asString().unwrapOr("Unknown error");
                if (error == "Authentication failed!" || error == "Unauthorized") {
                    LoginSettingNodeV3::logOut();
                    GDDLLoginLayer::create()->show();
                    Notification::create("Your session expired, log in again", NotificationIcon::Warning, 2)->show();
                    return;
                }
                // cache that no submission was made
                RatingsManager::cacheSubmission(this->gddlLevelID, Submission());
                Notification::create(error, NotificationIcon::Error, 2)->show();
            }
        }
        else if (e->isCancelled()) {
            // cache that no submission was made
            RatingsManager::cacheSubmission(this->gddlLevelID, Submission());
            Notification::create("An error occurred", NotificationIcon::Error, 2)->show();
        }
    });
    userSearchListener.bind([this](web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            if (res->code() == 200) {
                const int id = GDDLLoginLayer::getUserIDFromUserSearchJSON(jsonResponse, requestedUsername);
                if (id > -1) {
                    submissionJson["secondPlayerID"] = id;
                    makeSubmissionRequest();
                } else {
                    Notification::create(id == -1 ? "Second player not found!" : "An error occurred", NotificationIcon::Error, 2)->show();
                }
            } else {
                const std::string error = jsonResponse["message"].asString().unwrapOr("Unknown error");
                Notification::create(error, NotificationIcon::Error, 2)->show();
            }
        }
        else if (e->isCancelled()) {
            Notification::create("An error occurred", NotificationIcon::Error, 2)->show();
        }
    });
    userSubmissionCheckListener.bind([this](web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            if (res->code() == 200) {
                // submission found, cache it
                const Submission submission = Submission(jsonResponse, false);
                RatingsManager::cacheSubmission(this->gddlLevelID, submission);
                showAlreadySubmittedWarning();
            } else {
                const std::string error = jsonResponse["message"].asString().unwrapOr("Unknown error");
                Notification::create(error, NotificationIcon::Warning, 2)->show();
                if (error == "Submission not found!") {
                    // save an empty one
                    RatingsManager::cacheSubmission(this->gddlLevelID, Submission());
                }
            }
        }
        else if (e->isCancelled()) {
            Notification::create("Check if already submitted failed", NotificationIcon::Warning, 2)->show();
        }
    });
}

bool GDDLRatingSubmissionLayer::isValidProof(const std::string& proofURL) {
    return std::ranges::any_of(validProofURLs, [proofURL](const std::string& validProofURL) {
        return proofURL.starts_with(validProofURL);
    });
}


std::string GDDLRatingSubmissionLayer::fillOutSubmissionJson() {
    submissionJson["levelID"] = this->gddlLevelID;
    if (time(nullptr) < Utils::API_SWITCH_TIME) {
        submissionJson["device"] = mobile ? 2 : 1;
    } else {
        submissionJson["device"] = mobile ? "mobile" : "pc";
    }
    const int correctedRating = std::min(std::max(0, Utils::getNumberTextfieldValue(ratingTextfield)), 35);
    if (correctedRating != 0) {
        submissionJson["rating"] = correctedRating;
    }
    const int correctedEnjoyment = std::min(
        std::max(-1, Utils::getNumberWithGivenDefaultTextfieldValue(enjoymentTextfield, -1)), 10);
    if (correctedEnjoyment != -1) {
        submissionJson["enjoyment"] = correctedEnjoyment;
    }
    if (correctedRating == 0 && correctedEnjoyment == -1) {
        return "Rating and enjoyment can't both be empty";
    }
    int correctedFPS = Utils::getNumberWithGivenDefaultTextfieldValue(fpsTextfield, -1);
    if (correctedFPS != -1 && correctedFPS < 30) {
        return "Incorrect FPS value";
    }
    correctedFPS = std::min(std::max(-1, correctedFPS), 9999);
    if (correctedFPS != -1) {
        submissionJson["refreshRate"] = correctedFPS;
    }
    std::string correctedProof = proofTextfield->getString();
    correctedProof.erase(correctedProof.begin(), std::ranges::find_if(correctedProof, [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    if (!correctedProof.empty()) {
        if (!isValidProof(correctedProof)) {
            return "Invalid proof URL";
        }
        submissionJson["proof"] = correctedProof;
    }
    else if (correctedRating >= 21) {
        return "Tier 21 and above submissions require proof";
    }
    const int correctedProgress = std::min(
        std::max(0, Utils::getNumberWithGivenDefaultTextfieldValue(percentTextfield, 100)), 100);
    if (correctedProgress == 0) {
        return "Progress can't be 0%";
    }
    submissionJson["progress"] = correctedProgress;
    if (const int correctedAttempts = std::min(
        std::max(-1, Utils::getNumberWithGivenDefaultTextfieldValue(attemptsTextfield, -1)),
        999999999); correctedAttempts != -1) {
        if (correctedAttempts == 0) {
            return "Attempts can't be 0";
        }
        submissionJson["attempts"] = correctedAttempts;
    }
    return "";
}

void GDDLRatingSubmissionLayer::makeSubmissionRequest() {
    // check if the user is logged in
    if (!LoginSettingNodeV3::loggedIn()) {
        GDDLLoginLayer::create()->show();
        Notification::create("You are not logged in!", NotificationIcon::Warning, 2)->show();
        return;
    }
    auto req = web::WebRequest();
    req.header("User-Agent", Utils::getUserAgent());
    req.bodyJSON(submissionJson);
    req.header("Cookie", fmt::format("gddl.sid.sig={}; gddl.sid={}",
                                     Mod::get()->getSavedValue<std::string>("login-sig", ""),
                                     Mod::get()->getSavedValue<std::string>("login-sid", "")));
    submissionListener.setFilter(req.post(submissionEndpoint));
}


GDDLRatingSubmissionLayer* GDDLRatingSubmissionLayer::create(GJGameLevel* level, int gddlLevelID) {
    if (const auto newLayer = new GDDLRatingSubmissionLayer(); newLayer != nullptr && newLayer->init(level, gddlLevelID)) {
        newLayer->autorelease();
        return newLayer;
    }
    else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLRatingSubmissionLayer::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
    // submission check should be here to make it appear on top
    if (LoginSettingNodeV3::loggedIn()) {
        if(RatingsManager::hasSubmission(this->gddlLevelID)) {
            if (!RatingsManager::getSubmission(this->gddlLevelID).isEmpty()) {
                showAlreadySubmittedWarning();
            }
        } else {
            int userID = Mod::get()->getSavedValue<int>("login-userid", 0);
            auto req = web::WebRequest();
            req.header("User-Agent", Utils::getUserAgent());
            userSubmissionCheckListener.setFilter(req.get(getUserSubmissionCheckEndpoint(userID, this->gddlLevelID)));
        }
    } else {
        // the user would probably like to log in at this point I guess
        GDDLLoginLayer::create()->show();
        Notification::create("You are not logged in!", NotificationIcon::Warning, 2)->show();
    }
}

std::string GDDLRatingSubmissionLayer::getUserSubmissionCheckEndpoint(int userID, int levelID) {
    return "https://gdladder.com/api/user/" + std::to_string(userID) + "/submissions/" + std::to_string(levelID);
}

void GDDLRatingSubmissionLayer::showAlreadySubmittedWarning() {
    auto submission = RatingsManager::getSubmission(this->gddlLevelID);
    if (submission.isEmpty()) return;
    std::string description = submission.describe();
    std::string text = "<co>You have already submitted a rating for this level</c>: " + description + " Submitting <cy>another rating</c> will <cr>overwrite</c> the previous one!";
    FLAlertLayer::create("Warning", text, "OK")->show();
}
