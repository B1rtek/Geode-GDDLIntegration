#include "LoginSettingNodeV3.h"
#include "layers/GDDLLoginLayer.h"
#include "Utils.h"

bool LoginSettingNodeV3::init(std::shared_ptr<DummySettingLoginV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width)) {
        return false;
    }

    // login status
    loginStatus = CCLabelBMFont::create("Not logged in", "chatFont.fnt");
    loginStatus->setScale(0.5f);
    loginStatus->setAnchorPoint({0.0f, 0.5f});
    loginStatus->setPosition({0.0f, 15.0f});
    this->getButtonMenu()->addChild(loginStatus);


    // login/logout button
    loginLogoutButton = CCMenuItemSpriteExtra::create(getLoginLogoutButtonSprite(true), this, menu_selector(LoginSettingNodeV3::onLoginLogoutButtonClicked));
    loginLogoutButton->setPosition({150.0f, 15.0f});
    this->getButtonMenu()->addChild(loginLogoutButton);

    this->getButtonMenu()->setContentWidth(200);
    this->getButtonMenu()->updateLayout();

    updateState(nullptr);

    return true;
}

void LoginSettingNodeV3::updateState(CCNode *invoker) {
    SettingNodeV3::updateState(invoker);
    if (loggedIn()) {
        const std::string username = Mod::get()->getSavedValue<std::string>("login-username", "");
        const std::string loggedInString = "Logged in as " + username;
        loginStatus->setString(loggedInString.c_str());
        loginLogoutButton->setSprite(getLoginLogoutButtonSprite(false));
    } else {
        loginStatus->setString("Not logged in");
        loginLogoutButton->setSprite(getLoginLogoutButtonSprite(true));
    }
    Utils::scaleLabelToWidth(loginStatus, 100.0f);
    this->getButtonMenu()->updateLayout();
}

void LoginSettingNodeV3::onLoginLogoutButtonClicked(CCObject *sender) {
    if (loggedIn()) {
        logOut();
        this->markChanged(nullptr);
    } else {
        const auto loginLayer = GDDLLoginLayer::create();
        loginLayer->setSettingNode(this);
        loginLayer->show();
    }
}

bool LoginSettingNodeV3::loggedIn() {
    return !Mod::get()->getSavedValue<std::string>("login-sig", "").empty();
}

void LoginSettingNodeV3::logOut() {
    const std::string emptyString;
    Mod::get()->setSavedValue("login-sid", emptyString);
    Mod::get()->setSavedValue("login-sig", emptyString);
    RatingsManager::clearSubmissionCache();
}

void LoginSettingNodeV3::onCommit() {

}

void LoginSettingNodeV3::onResetToDefault() {

}

bool LoginSettingNodeV3::hasUncommittedChanges() const {
    return false;
}

bool LoginSettingNodeV3::hasNonDefaultValue() const {
    return false;
}

LoginSettingNodeV3 *LoginSettingNodeV3::create(std::shared_ptr<DummySettingLoginV3> setting, float width) {
    auto ret = new LoginSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void LoginSettingNodeV3::updateFromOutside() {
    this->markChanged(nullptr);
}

ButtonSprite *LoginSettingNodeV3::getLoginLogoutButtonSprite(bool login) {
    ButtonSprite* loginButtonSprite = ButtonSprite::create(login ? "Log in" : "Log out", "bigFont.fnt", login ? "GJ_button_01.png" : "GJ_button_06.png");
    loginButtonSprite->setScale(0.6f);
    return loginButtonSprite;
}
