#ifndef GDDLSEARCHLAYER_H
#define GDDLSEARCHLAYER_H

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class GDDLSearchLayer : public FLAlertLayer {
    bool init();
    void onClose(CCObject* sender);
    void onInfo(CCObject* sender);
    void onTierSearch(CCObject* sender);
    CCMenuItemSpriteExtra* createTierNode(int tier);
    CCMenu* createCheckboxNode(std::string idSuffix, std::string name);
public:
    static GDDLSearchLayer* create();
    void show();
};


#endif // GDDLSEARCHLAYER_H
