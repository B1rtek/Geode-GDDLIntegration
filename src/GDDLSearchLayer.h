#ifndef GDDLSEARCHLAYER_H
#define GDDLSEARCHLAYER_H

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class GDDLSearchLayer : public FLAlertLayer {
    bool init() override;
    void onClose(CCObject* sender);
    void onInfo(CCObject* sender);
    void onSearchOptionSelected(CCObject* sender);
    void onTierSearch(CCObject* sender);
    CCMenuItemSpriteExtra* createTierNode(int tier);
    CCMenu* createCheckboxNode(const std::string &idSuffix, const std::string &name);
public:
    static GDDLSearchLayer* create();
    void show() override;
};


#endif // GDDLSEARCHLAYER_H
