#ifndef GDDLINTEGRATION_GDDLHOMELAYER_H
#define GDDLINTEGRATION_GDDLHOMELAYER_H

#include "GDDLThemeBaseLayer.h"

using namespace geode::prelude;

class GDDLHomeLayer : public GDDLThemeBaseLayer {
protected:
    bool init() override;

    // void backActions() override;
public:
    static GDDLHomeLayer* create();
    static GDDLHomeLayer* scene();
};


#endif //GDDLINTEGRATION_GDDLHOMELAYER_H