#ifndef GDDLDEMONSPLITLAYER_H
#define GDDLDEMONSPLITLAYER_H

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class GDDLDemonSplitLayer final : public FLAlertLayer {
    CCMenuItemSpriteExtra* m_closeBtn{};

    bool init() override;
    void onClose(cocos2d::CCObject* sender);
    void keyBackClicked() override;
    void backActions();
    void onInfo(cocos2d::CCObject *sender);
    void onTierSearch(cocos2d::CCObject* sender);
    void onEnter() override;

    CCNode* createTierNode(int tier);
    void showLoadingCircle();
public:
    bool wasClosed = false;

    static GDDLDemonSplitLayer* create();
    void show() override;

    void hideLoadingCircle();
};



#endif //GDDLDEMONSPLITLAYER_H
