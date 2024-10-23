#ifndef GDDLBOOMSCROLLLAYER_H
#define GDDLBOOMSCROLLLAYER_H

#include <Geode/Bindings.hpp>
#include <Geode/modify/BoomScrollLayer.hpp>

#include "LevelSelectLayer.cpp"


using namespace geode::prelude;

struct GDDLBoomScrollLayer : public geode::Modify<GDDLBoomScrollLayer, BoomScrollLayer> {
    struct Fields {
      static inline GDDLRobtopLevelsLayer* robtopLevelsLayer = nullptr;
    };

    void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) override;
};

#endif //GDDLBOOMSCROLLLAYER_H
