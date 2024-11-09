#ifndef GDDLINTEGRATION_GDDLLOGINLAYER_H
#define GDDLINTEGRATION_GDDLLOGINLAYER_H

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "settings/LoginSettingNodeV3.h"

using namespace geode::prelude;

class GDDLLoginLayer final : public FLAlertLayer {
    CCMenuItemSpriteExtra* m_closeBtn{};
    CCLabelBMFont* statusLabel = nullptr;
    CCTextInputNode* usernameTextField = nullptr;
    CCTextInputNode* passwordTextField = nullptr;
    CCMenuItemSpriteExtra* loginButton = nullptr;

    const inline static std::string loginEndpoint = "https://gdladder.com/api/login";
    matjson::Value reqJson;
    EventListener<web::WebTask> loginListener;
    LoginSettingNodeV3* settingNode;

    bool init() override;
    void onClose(cocos2d::CCObject* sender);
    void onLoginClicked(cocos2d::CCObject *sender);

    void prepareSearchListener();
    void saveLoginData(const std::string& sid, const std::string& sig);
    void closeLoginPanel();
    void showLoadingCircle();
    void hideLoadingCircle();

    // getting around geode::web limitations
    std::pair<std::string, std::string> getCookieValue(const char* content);
    static size_t writeCallback(char *contents, size_t size, size_t nmemb, void *userp);
    std::thread spawnLoginRequestThread();

public:
    static GDDLLoginLayer* create();
    void show() override;

    void setSettingNode(LoginSettingNodeV3* settingNode);
};


#endif //GDDLINTEGRATION_GDDLLOGINLAYER_H
