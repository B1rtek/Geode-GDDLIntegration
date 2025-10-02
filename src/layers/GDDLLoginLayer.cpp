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
    const auto title = CCLabelBMFont::create("GDDL Login", "goldFont.fnt");
    title->setID("gddl-login-title"_spr);
    title->setPosition({popupSize.x / 2, popupSize.y - 20.0f});
    m_buttonMenu->addChild(title);
    // close button
    const auto closeButtonSprite = Utils::getGrayPopupCloseButton();
    m_closeBtn = CCMenuItemSpriteExtra::create(closeButtonSprite, this, menu_selector(GDDLLoginLayer::onClose));
    m_buttonMenu->addChild(m_closeBtn);
    m_closeBtn->setPosition({3.0f, popupSize.y - 3.0f});

    // content
    // status text
    statusLabel = CCLabelBMFont::create("", "chatFont.fnt");
    statusLabel->setID("gddl-login-status-label"_spr);
    statusLabel->setPosition({popupSize.x / 2, popupSize.y - 38.0f});
    statusLabel->setScale(0.7f);
    m_buttonMenu->addChild(statusLabel);
    // username text + textfield
    const auto usernameLabel = CCLabelBMFont::create("Username", "bigFont.fnt");
    usernameLabel->setScale(0.5f);
    usernameLabel->setID("gddl-login-username-label"_spr);
    usernameLabel->setPosition({popupSize.x / 2 - 60.0f, popupSize.y - 50.0f});
    m_buttonMenu->addChild(usernameLabel);
    Utils::createTextInputNode(m_buttonMenu, usernameTextField, "bigFont.fnt", "", 64, {200.0f, 25.0f},
                               {popupSize.x / 2, popupSize.y - 75.0f});
    usernameTextField->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
    usernameTextField->setString(Mod::get()->getSavedValue<std::string>("login-username", ""));
    // password text + textfield
    const auto passwordLabel = CCLabelBMFont::create("Password", "bigFont.fnt");
    passwordLabel->setScale(0.5f);
    passwordLabel->setID("gddl-login-password-label"_spr);
    passwordLabel->setPosition({popupSize.x / 2 - 60.0f, popupSize.y - 100.0f});
    m_buttonMenu->addChild(passwordLabel);
    Utils::createTextInputNode(m_buttonMenu, passwordTextField, "bigFont.fnt", "", 64, {200.0f, 25.0f},
                               {popupSize.x / 2, popupSize.y - 125.0f});
    passwordTextField->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
    passwordTextField->m_usePasswordChar = true;
    // login button
    const auto loginButtonSprite = ButtonSprite::create("Log in", "bigFont.fnt", "GJ_button_02.png");
    loginButtonSprite->setScale(0.6f);
    loginButton = CCMenuItemSpriteExtra::create(loginButtonSprite, this,
                                                           menu_selector(GDDLLoginLayer::onLoginClicked));
    loginButton->setID("gddl-login-login-button"_spr);
    loginButton->setPosition({popupSize.x / 2, popupSize.y - 160.0f});
    m_buttonMenu->addChild(loginButton);
    m_buttonMenu->reorderChild(loginButton, 10);

    prepareSearchListener();
    prepareMeListener();

    return true;
}

void GDDLLoginLayer::onClose(cocos2d::CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLLoginLayer::onLoginClicked(cocos2d::CCObject *sender) {
    matjson::Value reqJson = matjson::Value();
    std::string username = std::string(usernameTextField->getString());
    if (username.empty()) {
        Notification::create("Missing username", NotificationIcon::Warning, 2)->show();
        return;
    }
    std::string password = std::string(passwordTextField->getString());
    if (password.empty()) {
        Notification::create("Missing password", NotificationIcon::Warning, 2)->show();
        return;
    }
    reqJson["username"] = username;
    reqJson["password"] = password;
    auto req = web::WebRequest();
    req.header("User-Agent", Utils::getUserAgent());
    req.bodyJSON(reqJson);
    showLoadingCircle();
    loginListener.setFilter(req.post(loginEndpoint));
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

// left here because once geode 4.0.0 comes out raw curl won't be needed
void GDDLLoginLayer::prepareSearchListener() {
    loginListener.bind([this](web::WebTask::Event *e) {
        if (web::WebResponse *res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            if (res->code() == 200) {
                saveLoginData("gddl.sid", 0);
                std::optional<std::vector<std::string>> cookies = res->getAllHeadersNamed("set-cookie");
                if (cookies.has_value()) {
                    std::map<std::string, std::string> cookiesMap;
                    for (const auto& cookie : cookies.value()) {
                        const auto [name, value] = getCookieValue(cookie.c_str());
                        cookiesMap[name] = value;
                    }
                    if (cookiesMap.contains("gddl.sid")) {
                        // get uid
                        saveLoginData(cookiesMap["gddl.sid"], 0);
                        auto req = web::WebRequest();
                        req.header("User-Agent", Utils::getUserAgent());
                        req.header("Cookie", fmt::format("gddl.sid={}", cookiesMap["gddl.sid"]));
                        showLoadingCircle();
                        log::info("GDDLLoginLayer::loginListener: obtained session cookie");
                        meListener.setFilter(req.get(meEndpoint));
                    } else {
                        hideLoadingCircle();
                        const std::string errorMessage = "Error during login - no session cookie received";
                        Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
                        const std::string headersList = getAllHeaders(res);
                        const std::string rawResponse = jsonResponse.contains("message") ? jsonResponse.dump(0) : res->string().unwrapOr("Response was not a valid string");
                        log::error("GDDLLoginLayer::loginListener: {}, received headers: {}, raw response: {}", errorMessage, headersList, rawResponse);
                    }
                } else {
                    hideLoadingCircle();
                    const std::string errorMessage = "Error during login - missing cookies";
                    Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
                    const std::string headersList = getAllHeaders(res);
                    const std::string rawResponse = jsonResponse.contains("message") ? jsonResponse.dump(0) : res->string().unwrapOr("Response was not a valid string");
                    log::error("GDDLLoginLayer::loginListener: {}, received headers: {}, raw response: {}", errorMessage, headersList, rawResponse);
                }
            } else {
                // not success!
                const std::string error = jsonResponse["message"].asString().unwrapOr("Error during login - unknown error");
                hideLoadingCircle();
                Notification::create(error, NotificationIcon::Error, 2)->show();
                const std::string rawResponse = jsonResponse.contains("message") ? jsonResponse.dump(0) : res->string().unwrapOr("Response was not a valid string");
                log::error("GDDLLoginLayer::loginListener: {}, raw response: {}", error, rawResponse);
            }
        } else if (e->isCancelled()) {
            hideLoadingCircle();
            const std::string errorMessage = "Error during login - request cancelled";
            Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
            log::error("GDDLLoginLayer::loginListener: {}", errorMessage);
        }
    });
}

void GDDLLoginLayer::prepareMeListener() {
    meListener.bind([this](web::WebTask::Event *e) {
        if (web::WebResponse *res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            if (res->code() == 200) {
                const int uid = jsonResponse["ID"].asInt().unwrapOr(-1);
                if (uid == -1) {
                    hideLoadingCircle();
                    const std::string errorMessage = "Error during login - failed to obtain the user id";
                    Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
                    const std::string rawResponse = res->string().unwrapOr("Response was not a valid string");
                    log::error("GDDLLoginLayer::meListener: {}, raw response: {}", errorMessage, rawResponse);
                } else {
                    saveLoginData(Mod::get()->getSavedValue<std::string>("login-sid", ""), uid);
                    RatingsManager::clearSubmissionCache();
                    Notification::create("Logged in!", NotificationIcon::Success, 2)->show();
                    log::info("GDDLLoginLayer::meListener: successfully logged in, UID: {}", uid);
                    closeLoginPanel();
                }
            }
        } else if (e->isCancelled()) {
            hideLoadingCircle();
            const std::string errorMessage = "Error during login - user id request cancelled";
            Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
            log::error("GDDLLoginLayer::meListener: {}", errorMessage);
        }
    });
}

void GDDLLoginLayer::saveLoginData(const std::string &sid, const int uid) {
    Mod::get()->setSavedValue("login-username", std::string(usernameTextField->getString()));
    Mod::get()->setSavedValue("login-sid", sid);
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
