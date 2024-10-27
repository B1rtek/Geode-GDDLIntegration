#ifndef GDDLINTEGRATION_GDDLLOGINLAYER_H
#define GDDLINTEGRATION_GDDLLOGINLAYER_H

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GDDLLoginLayer final : public FLAlertLayer {
    CCMenuItemSpriteExtra* m_closeBtn{};
    CCTextInputNode* usernameTextField = nullptr;
    CCTextInputNode* passwordTextField = nullptr;

    bool init() override;
    void onClose(cocos2d::CCObject* sender);
    void onLoginClicked(cocos2d::CCObject *sender);

public:
    static GDDLLoginLayer* create();
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLLOGINLAYER_H
