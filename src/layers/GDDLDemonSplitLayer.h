#ifndef GDDLDEMONSPLITLAYER_H
#define GDDLDEMONSPLITLAYER_H

#include <Values.h>
#include <Geode/Bindings.hpp>
#include <objects/SearchObject.h>

using namespace geode::prelude;

class GDDLDemonSplitLayer final : public FLAlertLayer {
    static constexpr int rows = 5;
    static constexpr int columns = Values::highestTier % rows == 0 ? Values::highestTier / rows : Values::highestTier / rows + 1;
    CCMenuItemSpriteExtra* m_closeBtn{};
    SearchObject searchObject;

    bool init() override;
    void onClose(cocos2d::CCObject* sender);
    void keyBackClicked() override;
    void backActions();
    void onInfo(cocos2d::CCObject *sender);
    void onTierSearch(cocos2d::CCObject* sender);
    void onEnter() override;

    CCNode* createTierNode(int tier, int count);
    void showLoadingCircle();
public:
    bool wasClosed = false;

    static GDDLDemonSplitLayer* create();
    void show() override;

    void hideLoadingCircle();
};



#endif //GDDLDEMONSPLITLAYER_H
