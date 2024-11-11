#include "GDDLLoginLayer.h"

#include "Utils.h"
#include "external/ca_bundle.h"
#include "GDDLRatingSubmissionLayer.h"
#include <Geode/Loader.hpp>
#include <curl/curl.h>
#include <Geode/ui/LoadingSpinner.hpp>

bool GDDLLoginLayer::init() {
    if (!FLAlertLayer::init(75)) return false; // that magic number is actually bg opacity btw

    const CCPoint popupSize = {280.0f, 185.0f};
    const auto winSize = CCDirector::sharedDirector()->getWinSize();

    // background
    const auto bg = CCScale9Sprite::create("GJ_square05.png", {0.0f, 0.0f, 80.0f, 80.0f});
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
    const auto closeButtonSprite = CircleButtonSprite::createWithSpriteFrameName("geode.loader/close.png", .85f,
                                                                                 CircleBaseColor::Gray);
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

    return true;
}

void GDDLLoginLayer::onClose(cocos2d::CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLLoginLayer::onLoginClicked(cocos2d::CCObject *sender) {
    reqJson = matjson::Value();
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
    showLoadingCircle();
    spawnLoginRequestThread().detach();
}

// left here because once geode 4.0.0 comes out raw curl won't be needed
void GDDLLoginLayer::prepareSearchListener() {
    loginListener.bind([this](web::WebTask::Event *e) {
        if (web::WebResponse *res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            if (res->code() == 200) {
                Notification::create("Logged in!", NotificationIcon::Success, 2)->show();
                saveLoginData("gddl.sid", "gddl.sid.sig");
                RatingsManager::clearSubmissionCache();
                closeLoginPanel();
            } else {
                // not success!
                std::string error = "Unknown error";
                if (jsonResponse.contains("error")) {
                    error = jsonResponse["error"].as_string();
                }
                hideLoadingCircle();
                Notification::create(error, NotificationIcon::Error, 2)->show();
            }
        } else if (e->isCancelled()) {
            hideLoadingCircle();
            Notification::create("An error occurred", NotificationIcon::Error, 2)->show();
        }
    });
    userIDListener.bind([this](web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            if (res->code() == 200) {
                log::debug("received the id json");
                const int id = GDDLLoginLayer::getUserIDFromUserSearchJSON(jsonResponse, Mod::get()->getSavedValue<std::string>("login-username", ""));
                log::debug("got the id from the json: {}", id);
                if (id > -1) {
                    Mod::get()->setSavedValue("login-userid", id);
                    log::debug("saved the id from the json: {}", id);
                    Notification::create("Logged in!", NotificationIcon::Success, 2)->show();
                    closeLoginPanel();
                } else {
                    Notification::create(id == -1 ? "Your player ID wasn't found, try relogging later" : "An error occurred", NotificationIcon::Error, 2)->show();
                }
            } else {
                Notification::create("Your player ID wasn't found, try relogging later", NotificationIcon::Error, 2)->show();
            }
        }
        else if (e->isCancelled()) {
            Notification::create("Your player ID wasn't found, try relogging later", NotificationIcon::Error, 2)->show();
        }
    });
}

void GDDLLoginLayer::saveLoginData(const std::string &sid, const std::string &sig) {
    Mod::get()->setSavedValue("login-username", std::string(usernameTextField->getString()));
    Mod::get()->setSavedValue("login-sid", sid);
    Mod::get()->setSavedValue("login-sig", sig);
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

size_t GDDLLoginLayer::writeCallback(char *contents, size_t size, size_t nmemb, void *userp) {
    // stack overflow yay https://stackoverflow.com/questions/44994203/how-to-get-the-http-response-string-using-curl-in-c
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::pair<std::string, std::string> GDDLLoginLayer::getCookieValue(const char *content) {
    std::string cookie = content;
    // get cookie name
    int equalsPos = cookie.find('=');
    std::string cookieName = cookie.substr(0, equalsPos);
    std::string cookieValue = cookie.substr(equalsPos + 1, cookie.find(';', equalsPos) - equalsPos - 1);
    return {cookieName, cookieValue};
}

std::thread GDDLLoginLayer::spawnLoginRequestThread() {
    return std::thread([this]() {
        CURL* curl = curl_easy_init();
        CURLcode res;
        struct curl_header *type;
        if (curl) {
            struct curl_slist *list = nullptr;
            std::string readBuffer;
            // set up the post request
            curl_easy_setopt(curl, CURLOPT_URL, loginEndpoint.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "GDDLIntegration");
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);
            // thank you dank once again <3 go support Globed!
            // https://github.com/GlobedGD/globed2/
            curl_blob cbb = {};
            cbb.data = const_cast<void*>(reinterpret_cast<const void*>(CA_BUNDLE_CONTENT));
            cbb.len = sizeof(CA_BUNDLE_CONTENT);
            cbb.flags = CURL_BLOB_COPY;
            curl_easy_setopt(curl, CURLOPT_CAINFO_BLOB, &cbb);
            // prepare data
            std::string strData = reqJson.dump();
            const char *data = strData.c_str();
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
            list = curl_slist_append(list, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDDLLoginLayer::writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            // dew it
            res = curl_easy_perform(curl);

            /* Check for errors */
            if(res != CURLE_OK) {
                hideLoadingCircle();
                Loader::get()->queueInMainThread([]() {
                    Notification::create("An error occurred", NotificationIcon::Error, 2)->show();
                });
                return;
            }

            long httpCode = 0;
            curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpCode);
            if (httpCode == 200) {
                // extract the cookies and log in
                CURLHcode h;
                int i = 0;
                std::map<std::string, std::string> cookies;
                while (true) {
                    h = curl_easy_header(curl, "Set-Cookie", i, CURLH_HEADER, -1, &type);
                    if (h != CURLHE_OK) {
                        break;
                    }
                    const auto cookieNameValuePair = getCookieValue(type->value);
                    cookies[cookieNameValuePair.first] = cookieNameValuePair.second;
                    ++i;
                }
                saveLoginData(cookies["gddl.sid"], cookies["gddl.sid.sig"]);
                Loader::get()->queueInMainThread([this]() {
                    // get the uid
                    std::string requestURL = GDDLRatingSubmissionLayer::userSearchEndpoint;
                    const std::string requestedUsername = Mod::get()->getSavedValue<std::string>("login-username", "");
                    requestURL += "?name=" + requestedUsername + "&chunk=25";
                    auto req = web::WebRequest();
                    userIDListener.setFilter(req.get(requestURL));
                });
            } else {
                // something went wrong - get the error
                std::string error = "Unknown error", parseError;
                const std::optional<matjson::Value> maybeJsonResponse = matjson::parse(readBuffer, parseError);
                if (maybeJsonResponse.has_value()) {
                    const auto jsonResponse = maybeJsonResponse.value();
                    if (jsonResponse.contains("error")) {
                        error = jsonResponse["error"].as_string();
                    }
                } else {
                    error = "Server returned an invalid response";
                }
                hideLoadingCircle();
                Loader::get()->queueInMainThread([error]() {
                    Notification::create(error, NotificationIcon::Error, 2)->show();
                });
            }

            curl_easy_cleanup(curl);
        }
    });
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
    if (!jsonResponse.is_array()) {
        return -2;
    }
    log::debug("it's an array, requested username: {}", requestedUsername);
    const auto resultsList = jsonResponse.as_array();
    int id = -1;
    for (const auto& result : resultsList) {
        if (!result.contains("Name") || !result.contains("ID")) {
            continue;
        }
        if (result["Name"] == requestedUsername) {
            id = result["ID"].as_int();
            break;
        }
    }
    return id;
}
