#ifndef GDDLINTEGRATION_GDDLSEARCHLAYERV2_H
#define GDDLINTEGRATION_GDDLSEARCHLAYERV2_H

#include <Geode/Bindings.hpp>
#include <Geode/utils/web.hpp>

class GDDLSearchLayerV2 : public FLAlertLayer {
    bool init() override;

public:
    static GDDLSearchLayerV2* create();
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLSEARCHLAYERV2_H