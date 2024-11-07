#include "GDDLLoginLayer.h"

#include "Utils.h"
#include <Geode/Loader.hpp>

bool GDDLLoginLayer::init() {
    if(!FLAlertLayer::init(75)) return false; // that magic number is actually bg opacity btw

    const CCPoint popupSize = {280.0f, 185.0f};
    const auto winSize = CCDirector::sharedDirector()->getWinSize();

    // background
    const auto bg = CCScale9Sprite::create("GJ_square05.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });
    bg->setID("gddl-login-bg"_spr);
    m_mainLayer->addChild(bg, -1);

    // menu with the main layout
    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("gddl-login-button-menu"_spr);
    m_buttonMenu->setContentSize(popupSize);
    m_buttonMenu->setPosition({ winSize.width / 2 - popupSize.x / 2, winSize.height / 2 - popupSize.y / 2});
    m_mainLayer->addChild(m_buttonMenu, 10);
    // title
    const auto title = CCLabelBMFont::create("GDDL Login", "goldFont.fnt");
    title->setID("gddl-login-title"_spr);
    title->setPosition({popupSize.x / 2, popupSize.y - 20.0f});
    m_buttonMenu->addChild(title);
    // close button
    const auto closeButtonSprite = CircleButtonSprite::createWithSpriteFrameName("geode.loader/close.png", .85f,CircleBaseColor::Gray);
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
    Utils::createTextInputNode(m_buttonMenu, usernameTextField, "bigFont.fnt", "", 32, {200.0f, 25.0f}, {popupSize.x / 2, popupSize.y - 75.0f});
    usernameTextField->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
    usernameTextField->setString(Mod::get()->getSavedValue<std::string>("login-username", ""));
    // password text + textfield
    const auto passwordLabel = CCLabelBMFont::create("Password", "bigFont.fnt");
    passwordLabel->setScale(0.5f);
    passwordLabel->setID("gddl-login-password-label"_spr);
    passwordLabel->setPosition({popupSize.x / 2 - 60.0f, popupSize.y - 100.0f});
    m_buttonMenu->addChild(passwordLabel);
    Utils::createTextInputNode(m_buttonMenu, passwordTextField, "bigFont.fnt", "", 32, {200.0f, 25.0f}, {popupSize.x / 2, popupSize.y - 125.0f});
    passwordTextField->setAllowedChars(Utils::hopefullyAllCharactersAnyoneWillEverNeed);
    passwordTextField->m_usePasswordChar = true;
    // login button
    const auto loginButtonSprite = ButtonSprite::create("Log in", "bigFont.fnt", "GJ_button_02.png");
    loginButtonSprite->setScale(0.6f);
    const auto loginButton = CCMenuItemSpriteExtra::create(loginButtonSprite, this, menu_selector(GDDLLoginLayer::onLoginClicked));
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
    auto req = web::WebRequest();
    auto reqJson = matjson::Object {
            {"username", usernameTextField->getString()},
            {"password", passwordTextField->getString()}
    };
    req.bodyJSON(reqJson);
    loginListener.setFilter(req.post(loginEndpoint));
    updateStatusLabel("Logging in...", false);
}

void GDDLLoginLayer::prepareSearchListener() {
    loginListener.bind([this] (web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            const auto jsonResponse = res->json().unwrapOr(matjson::Value());
            if (res->code() == 200) {
                updateStatusLabel("Success!", false);
                saveLoginData("gddl.sid", "gddl.sid.sig");
                closeLoginPanel();
            } else {
                // not success!
                std::string error = "Unknown error";
                if(jsonResponse.contains("error")) {
                    error = jsonResponse["error"].as_string();
                }
                updateStatusLabel(error, true);
            }
        } else if (e->isCancelled()) {
            updateStatusLabel("An error occurred", true);
        }
    });
}

void GDDLLoginLayer::updateStatusLabel(const std::string& newStatus, bool error) {
    statusLabel->setString(newStatus.c_str());
    statusLabel->setColor(error ? ccc3(237, 67, 55) : ccc3(255, 255, 255));
}

void GDDLLoginLayer::saveLoginData(const std::string &sid, const std::string &sig) {
    Mod::get()->setSavedValue("login-username", usernameTextField->getString());
    Mod::get()->setSavedValue("login-sid", sid);
    Mod::get()->setSavedValue("login-sig", sig);
}

void GDDLLoginLayer::closeLoginPanel() {
    if (settingNode != nullptr) {
        settingNode->updateFromOutside();
    }
    onClose(nullptr);
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
