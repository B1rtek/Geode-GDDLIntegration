#include "GDDLLoginLayer.h"

#include "Utils.h"
#include "GDDLRatingSubmissionLayer.h"
#include <Geode/Loader.hpp>
#include <Geode/ui/LoadingSpinner.hpp>

bool GDDLLoginLayer::init() {
    if (!FLAlertLayer::init(75)) return false; // that magic number is actually bg opacity btw

    const CCPoint popupSize = {280.0f, 185.0f};
    const auto winSize = CCDirector::sharedDirector()->getWinSize();

    // background
    const auto bg = CCScale9Sprite::create(Utils::getGrayPopupBG().c_str(), {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({winSize.width / 2, winSize.height / 2});
    bg->setID("gddl-login-bg"_spr);
    m_mainLayer->addChild(bg, -1);

    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("gddl-login-button-menu"_spr);
    m_buttonMenu->setContentSize(popupSize);
    m_buttonMenu->setPosition({winSize.width / 2 - popupSize.x / 2, winSize.height / 2 - popupSize.y / 2});
    m_mainLayer->addChild(m_buttonMenu, 10);

    // title
    const auto titleContent = LoginSettingNodeV3::loggedIn() ? "Manage API key" : "Log in with API key";
    const auto title = CCLabelBMFont::create(titleContent, "goldFont.fnt");
    title->setScale(LoginSettingNodeV3::loggedIn() ? 1.0f : 0.9f);
    title->setID("gddl-login-title"_spr);
    title->setPosition({popupSize.x / 2, popupSize.y - 20.0f});
    m_buttonMenu->addChild(title);

    // close button
    const auto closeButtonSprite = Utils::getGrayPopupCloseButton();
    m_closeBtn = CCMenuItemSpriteExtra::create(closeButtonSprite, this, menu_selector(GDDLLoginLayer::onClose));
    m_buttonMenu->addChild(m_closeBtn);
    m_closeBtn->setPosition({3.0f, popupSize.y - 3.0f});

    // content of the popup depends on whether the user is logged in or not
    if (LoginSettingNodeV3::loggedIn()) {
        // logged in label
        const std::string username = Mod::get()->getSavedValue<std::string>("login-username", "");
        const auto loggedInLabel = CCLabelBMFont::create(("Logged in as " + username).c_str(), "bigFont.fnt");
        loggedInLabel->setScale(0.5f);
        loggedInLabel->setID("gddl-login-logged-in-label"_spr);
        loggedInLabel->setPosition({popupSize.x / 2, popupSize.y - 50.0f});
        m_buttonMenu->addChild(loggedInLabel);

        // copy key button
        const auto copyKeyButtonSprite = ButtonSprite::create("Copy key", "bigFont.fnt", "GJ_button_02.png");
        copyKeyButtonSprite->setScale(0.6f);
        loginButton = CCMenuItemSpriteExtra::create(copyKeyButtonSprite, this,
                                                               menu_selector(GDDLLoginLayer::onCopyAPIKeyClicked));
        loginButton->setID("gddl-login-copy-key-button"_spr);
        loginButton->setPosition({popupSize.x / 2 - 60.0f, popupSize.y - 80.0f});
        m_buttonMenu->addChild(loginButton);
        m_buttonMenu->reorderChild(loginButton, 10);

        // log out button
        const auto logOutButtonSprite = ButtonSprite::create("Log out", "bigFont.fnt", "GJ_button_06.png");
        logOutButtonSprite->setScale(0.6f);
        loginButton = CCMenuItemSpriteExtra::create(logOutButtonSprite, this,
                                                               menu_selector(GDDLLoginLayer::onLogOutClicked));
        loginButton->setID("gddl-login-log-out-button"_spr);
        loginButton->setPosition({popupSize.x / 2 + 60.0f, popupSize.y - 80.0f});
        m_buttonMenu->addChild(loginButton);
        m_buttonMenu->reorderChild(loginButton, 10);

        // disclaimer about logging out
        const std::string disclaimerMessage = "<cr>Logging out</c> will <cy>deactivate</c> the <cb>API key</c> in use,\nlogging out <cy>all devices using this key</c>.";
        const auto disclaimerTextArea = TextArea::create(disclaimerMessage, "chatFont.fnt", 0.8, popupSize.x, {0.5f, 0.5f}, 12, false);
        disclaimerTextArea->setID("gddl-login-disclaimer"_spr);
        disclaimerTextArea->setPosition({popupSize.x / 2 + 30.0f, popupSize.y - 110.0f}); // why does this thing not place itself in the middle ugh
        m_buttonMenu->addChild(disclaimerTextArea);
    } else {
        // api key input field
        Utils::createTextInputNode(m_buttonMenu, apiKeyTextField, "bigFont.fnt", "", 64, {200.0f, 25.0f},
                               {popupSize.x / 2, popupSize.y - 60.0f});
        apiKeyTextField->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
        apiKeyTextField->m_usePasswordChar = true;

        // log in button
        const auto copyKeyButtonSprite = ButtonSprite::create("Log in", "bigFont.fnt", "GJ_button_01.png");
        copyKeyButtonSprite->setScale(0.6f);
        loginButton = CCMenuItemSpriteExtra::create(copyKeyButtonSprite, this,
                                                               menu_selector(GDDLLoginLayer::onLoginClicked));
        loginButton->setID("gddl-login-log-in-button"_spr);
        loginButton->setPosition({popupSize.x / 2, popupSize.y - 90.0f});
        m_buttonMenu->addChild(loginButton);
        m_buttonMenu->reorderChild(loginButton, 10);

        // instructions
        const std::string apiKeyInstructions = "To obtain your <cb>API key</c>, log into your <cr>GDDL account</c>\nin the browser and go to <cg>Settings</c> > <cp>Developer</c>.";
        const auto apiKeyTextArea = TextArea::create(apiKeyInstructions, "chatFont.fnt", 0.8, popupSize.x, {0.5f, 0.5f}, 12, false);
        apiKeyTextArea->setID("gddl-login-api-key-instructions"_spr);
        apiKeyTextArea->setPosition({popupSize.x / 2 + 35.0f, popupSize.y - 120.0f}); // why does this thing not place itself in the middle ugh
        m_buttonMenu->addChild(apiKeyTextArea);
    }

    // never share this key with anyone label
    std::string neverShareMessage = "<cr>Never</c> share your <cb>API key</c> with anyone.\n<co>Anyone</c> in the possession of your <cb>API key</c>\nhas <cy>full access to your account</c>.";
    const auto disclaimerTextArea = TextArea::create(neverShareMessage, "chatFont.fnt", 0.8, popupSize.x, {0.5f, 0.5f}, 12, false);
    disclaimerTextArea->setID("gddl-login-never-share"_spr);
    disclaimerTextArea->setPosition({popupSize.x / 2 + 25.0f, popupSize.y - 155.0f}); // why does this thing not place itself in the middle ugh
    m_buttonMenu->addChild(disclaimerTextArea);

    prepareMeListener();

    return true;
}

void GDDLLoginLayer::onClose(cocos2d::CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLLoginLayer::onLoginClicked(cocos2d::CCObject *sender) {
    const std::string authHeader = "Bearer " + apiKeyTextField->getString();
    auto req = web::WebRequest();
    req.header("Authorization", authHeader);
    req.header("User-Agent", Utils::getUserAgent());
    showLoadingCircle();
    // if /api/user/me returns 200/201, the key is valid for the fetched user
    saveLoginData("", 0);
    meListener.setFilter(req.get(meEndpoint));
}

void GDDLLoginLayer::onCopyAPIKeyClicked(cocos2d::CCObject* sender) {

}

void GDDLLoginLayer::onLogOutClicked(cocos2d::CCObject* sender) {

}

std::string GDDLLoginLayer::getAllHeaders(web::WebResponse* response) {
    const auto headers = response->headers();
    std::string headersList = "";
    for (int i = 0; i < headers.size(); ++i) {
        headersList += headers[i];
        if (i != headers.size() - 1) {
            headersList += ", ";
        }
    }
    return headersList;
}

void GDDLLoginLayer::prepareMeListener() {
    meListener.bind([this](web::WebTask::Event *e) {
        if (web::WebResponse *res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            if (res->code() == 200) {
                const int uid = jsonResponse["ID"].asInt().unwrapOr(-1);
                const std::string username = jsonResponse["Name"].asString().unwrapOr("");
                if (uid == -1 || username == "") {
                    hideLoadingCircle();
                    const std::string errorMessage = "Error during login - failed to obtain the user id or username";
                    Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
                    const std::string rawResponse = res->string().unwrapOr("Response was not a valid string");
                    log::error("GDDLLoginLayer::meListener: {}, raw response: {}", errorMessage, rawResponse);
                } else {
                    saveLoginData(username, uid);
                    RatingsManager::clearSubmissionCache();
                    Notification::create("Logged in!", NotificationIcon::Success, 2)->show();
                    log::info("GDDLLoginLayer::meListener: successfully logged in, UID: {}", uid);
                    closeLoginPanel();
                }
            } else {
                hideLoadingCircle();
                const std::string errorMessage = "Error during login - " + Utils::getErrorFromMessageAndResponse(jsonResponse, res);
                Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
                log::error("GDDLLevelInfoPopup::meListener: [{}] {}", res->code(), errorMessage);
            }
        } else if (e->isCancelled()) {
            hideLoadingCircle();
            const std::string errorMessage = "Error during login - request cancelled";
            Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
            log::error("GDDLLoginLayer::meListener: {}", errorMessage);
        }
    });
}

void GDDLLoginLayer::saveLoginData(const std::string &username, const int uid) {
    Mod::get()->setSavedValue("api-key", username.empty() ? "" : std::string(apiKeyTextField->getString()));
    Mod::get()->setSavedValue("login-username", username);
    Mod::get()->setSavedValue("login-userid", uid);
}

void GDDLLoginLayer::closeLoginPanel() {
    if (settingNode != nullptr) {
        settingNode->updateFromOutside();
    }
    onClose(nullptr);
}

void GDDLLoginLayer::showLoadingCircle() {
    const auto loadingSpinner = LoadingSpinner::create(15.0f);
    loadingSpinner->setAnchorPoint({0.0f, 0.5f});
    loadingSpinner->setPosition({loginButton->getPositionX() + loginButton->getScaledContentWidth() / 2 + 5.0f, loginButton->getPositionY()});
    loadingSpinner->setID("gddl-login-loading-spinner"_spr);
    m_buttonMenu->addChild(loadingSpinner);
}

void GDDLLoginLayer::hideLoadingCircle() {
    m_buttonMenu->removeChildByID("gddl-login-loading-spinner"_spr);
}

std::pair<std::string, std::string> GDDLLoginLayer::getCookieValue(const char *content) {
    std::string cookie = content;
    // get cookie name
    int equalsPos = cookie.find('=');
    std::string cookieName = cookie.substr(0, equalsPos);
    std::string cookieValue = cookie.substr(equalsPos + 1, cookie.find(';', equalsPos) - equalsPos - 1);
    return {cookieName, cookieValue};
}

GDDLLoginLayer *GDDLLoginLayer::create() {
    if (const auto newLayer = new GDDLLoginLayer(); newLayer != nullptr && newLayer->init()) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLLoginLayer::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}

void GDDLLoginLayer::setSettingNode(LoginSettingNodeV3 *settingNode) {
    this->settingNode = settingNode;
}

// -2 = error, -1 = not found, >0 = uid
int GDDLLoginLayer::getUserIDFromUserSearchJSON(matjson::Value jsonResponse, const std::string& requestedUsername) {
    if (!jsonResponse.isArray()) {
        return -2;
    }
    const auto resultsList = jsonResponse.asArray().unwrap();
    int id = -1;
    for (const auto& result : resultsList) {
        if (!result.contains("ID") || !result["ID"].isNumber()) {
            continue;
        }
        if (result["Name"].asString().unwrapOr("") == requestedUsername) {
            id = result["ID"].asInt().unwrap();
            break;
        }
    }
    return id;
}
