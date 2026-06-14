#include "GDDLHomeLayer.h"

bool GDDLHomeLayer::init() {
    if (!GDDLThemeBaseLayer::init()) {
        return false;
    }

    return true;
}

GDDLHomeLayer* GDDLHomeLayer::create() {
    const auto ret = new GDDLHomeLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

GDDLHomeLayer* GDDLHomeLayer::scene() {
    const auto scene = CCScene::create();
    const auto layer = create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
    return layer;
}
