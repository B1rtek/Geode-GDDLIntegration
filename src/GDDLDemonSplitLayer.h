#ifndef GDDLDEMONSPLITLAYER_H
#define GDDLDEMONSPLITLAYER_H

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class GDDLDemonSplitLayer : public FLAlertLayer {
    CCMenuItemSpriteExtra* m_closeBtn{};

    bool init();
    void onClose(cocos2d::CCObject* sender);
public:
    static GDDLDemonSplitLayer* create();
};



#endif //GDDLDEMONSPLITLAYER_H
