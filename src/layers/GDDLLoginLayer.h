#ifndef GDDLINTEGRATION_GDDLLOGINLAYER_H
#define GDDLINTEGRATION_GDDLLOGINLAYER_H

#include <Utils.h>
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "settings/LoginSettingNodeV3.h"

using namespace geode::prelude;

class GDDLLoginLayer final : public FLAlertLayer {
    CCMenuItemSpriteExtra* m_closeBtn{};
    CCLabelBMFont* statusLabel = nullptr;
    CCTextInputNode* apiKeyTextField = nullptr;
    CCMenuItemSpriteExtra* loginButton = nullptr;

    const inline static std::string loginEndpoint = "https://gdladder.com/api/account/login";
    const inline static std::string meEndpoint = "https://gdladder.com/api/user/me";
    async::TaskHolder<web::WebResponse> meListener;
    LoginSettingNodeV3* settingNode;

    bool init() override;
    void onClose(cocos2d::CCObject* sender);
    void onLoginClicked(cocos2d::CCObject *sender);
    void onCopyAPIKeyClicked(cocos2d::CCObject *sender);
    void onLogOutClicked(cocos2d::CCObject *sender);

    std::function<void(web::WebResponse)> getMeListenerLambda();
    void saveLoginData(const std::string& username, int uid);
    void closeLoginPanel();
    void showLoadingCircle();
    void hideLoadingCircle();

public:
    static GDDLLoginLayer* create();
    void show() override;

    void setSettingNode(LoginSettingNodeV3* settingNode);
    static int getUserIDFromUserSearchJSON(matjson::Value jsonResponse, const std::string& requestedUsername);
};


#endif //GDDLINTEGRATION_GDDLLOGINLAYER_H
