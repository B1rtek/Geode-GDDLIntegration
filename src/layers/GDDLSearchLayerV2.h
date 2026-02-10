#ifndef GDDLINTEGRATION_GDDLSEARCHLAYERV2_H
#define GDDLINTEGRATION_GDDLSEARCHLAYERV2_H

#include <Geode/Bindings.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class GDDLSearchLayerV2 : public FLAlertLayer {
    const CCPoint popupSize = {440.0f, 290.0f};

    bool init() override;
    void onClose(CCObject *sender);
public:
    static GDDLSearchLayerV2* create();
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLSEARCHLAYERV2_H