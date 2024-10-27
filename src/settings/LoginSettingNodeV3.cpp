#include "LoginSettingNodeV3.h"
#include "layers/GDDLLoginLayer.h"

bool LoginSettingNodeV3::init(std::shared_ptr<DummySettingLoginV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width)) {
        return false;
    }

    // login/logout button
    const auto loginButtonSprite = ButtonSprite::create("Log in", "bigFont.fnt", "GJ_button_01.png");
    loginButtonSprite->setScale(0.6f);
    const auto resetButton = CCMenuItemSpriteExtra::create(loginButtonSprite, this, menu_selector(LoginSettingNodeV3::onLoginLogoutButtonClicked));

    this->getButtonMenu()->addChildAtPosition(resetButton, Anchor::Center);
    this->getButtonMenu()->setContentWidth(150);
    this->getButtonMenu()->updateLayout();

    return true;
}

void LoginSettingNodeV3::updateState(CCNode *invoker) {
    SettingNodeV3::updateState(invoker);
    std::cout << "updateState called" << std::endl;
}

void LoginSettingNodeV3::onLoginLogoutButtonClicked(CCObject *sender) {
    GDDLLoginLayer::create()->show();
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
