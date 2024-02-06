#ifndef GDDLDEMONSPLITLAYER_H
#define GDDLDEMONSPLITLAYER_H

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class GDDLDemonSplitLayer : public FLAlertLayer {
    CCMenuItemSpriteExtra* m_closeBtn{};

    bool init();
    void onClose(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject *sender);
    void onTierSearch(cocos2d::CCObject* sender);

    CCNode* createTierNode(int tier);
public:
    static GDDLDemonSplitLayer* create();
    void show();
};



#endif //GDDLDEMONSPLITLAYER_H
