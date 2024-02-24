#ifndef GDDLDEMONSPLITLAYER_H
#define GDDLDEMONSPLITLAYER_H

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class GDDLDemonSplitLayer final : public FLAlertLayer {
    CCMenuItemSpriteExtra* m_closeBtn{};

    bool init() override;
    void onClose(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject *sender);
    void onTierSearch(cocos2d::CCObject* sender);
    void onEnter() override;

    CCNode* createTierNode(int tier);
public:
    static GDDLDemonSplitLayer* create();
    void show() override;
};



#endif //GDDLDEMONSPLITLAYER_H
