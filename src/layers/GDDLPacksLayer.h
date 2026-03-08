#ifndef GDDLINTEGRATION_GDDLPACKSLAYER_H
#define GDDLINTEGRATION_GDDLPACKSLAYER_H

#include <Geode/Bindings.hpp>
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

class GDDLPacksLayer : public CCLayer {
    static constexpr CCPoint listSize = {356.0f, 220.0f};

    ScrollLayer* packsList = nullptr;

    bool init() override;

    void createListFrame();

    void onBack(CCObject* sender);
    void keyBackClicked() override;
    void backActions();
public:

    static GDDLPacksLayer* create();
    static GDDLPacksLayer* scene();
};


#endif //GDDLINTEGRATION_GDDLPACKSLAYER_H