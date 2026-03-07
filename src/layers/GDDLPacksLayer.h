#ifndef GDDLINTEGRATION_GDDLPACKSLAYER_H
#define GDDLINTEGRATION_GDDLPACKSLAYER_H

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class GDDLPacksLayer : public CCLayer {
    bool init() override;

    void onBack(CCObject* sender);
public:
    static GDDLPacksLayer* create();
    static GDDLPacksLayer* scene();
};


#endif //GDDLINTEGRATION_GDDLPACKSLAYER_H