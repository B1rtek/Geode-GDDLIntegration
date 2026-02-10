#ifndef GDDLINTEGRATION_GDDLSEARCHLAYERV2_H
#define GDDLINTEGRATION_GDDLSEARCHLAYERV2_H

#include <Geode/Bindings.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class GDDLSearchLayerV2 : public FLAlertLayer {
    const CCPoint popupSize = {440.0f, 290.0f};

    // normal popup stuff
    bool init() override;
    void onClose(CCObject *sender);

    // internal ui methods
    void createBaseUI();
public:
    // normal popup stuff
    static GDDLSearchLayerV2* create();
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLSEARCHLAYERV2_H