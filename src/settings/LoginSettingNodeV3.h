#ifndef GDDLINTEGRATION_LOGINSETTINGNODEV3_H
#define GDDLINTEGRATION_LOGINSETTINGNODEV3_H

#include <Geode/loader/SettingV3.hpp>
#include <Geode/utils/web.hpp>

#include "DummySettingLoginV3.h"

using namespace geode::prelude;

class LoginSettingNodeV3 : public SettingNodeV3 {
    const inline static std::string logoutEndpoint = "https://gdladder.com/api/account/logout";
    EventListener<web::WebTask> logoutListener;

    void prepareLogoutListener();
protected:
    CCLabelBMFont* loginStatus = nullptr;
    CCMenuItemSpriteExtra* loginLogoutButton = nullptr;

    bool init(std::shared_ptr<DummySettingLoginV3> setting, float width);
    void updateState(CCNode* invoker) override;

    void onLoginLogoutButtonClicked(CCObject *sender);
    ButtonSprite* getLoginLogoutButtonSprite(bool login);

    void onCommit() override;
    void onResetToDefault() override;

public:
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;

    void updateFromOutside();
    static bool loggedIn();
    static void logOut();

    static LoginSettingNodeV3* create(std::shared_ptr<DummySettingLoginV3> setting, float width);
};


#endif //GDDLINTEGRATION_LOGINSETTINGNODEV3_H
