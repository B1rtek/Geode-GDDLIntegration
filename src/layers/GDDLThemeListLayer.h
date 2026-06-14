#ifndef GDDLINTEGRATION_GDDLTHEMELISTLAYER_H
#define GDDLINTEGRATION_GDDLTHEMELISTLAYER_H

#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include "GDDLThemeBaseLayer.h"

using namespace geode::prelude;


class GDDLThemeListLayer : public GDDLThemeBaseLayer {
protected:
    static constexpr CCPoint listSize = {356.0f, 220.0f};
    ScrollLayer* packsList = nullptr;

    bool init(bool thickFrame = true);

    void createThinFrame();
    void createThickFrame();
public:
    static GDDLThemeListLayer* create(bool thickFrame = true);
    static GDDLThemeListLayer* scene(bool thickFrame = true);
};


#endif //GDDLINTEGRATION_GDDLTHEMELISTLAYER_H