#ifndef GDDLINTEGRATION_GDDLTHEMEBASELAYER_H
#define GDDLINTEGRATION_GDDLTHEMEBASELAYER_H

#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class GDDLThemeBaseLayer : public CCLayer {
protected:
    bool init() override;

    void onBack(CCObject* sender);
    void keyBackClicked() override;
    virtual void backActions();

public:
    static GDDLThemeBaseLayer* create();
    static GDDLThemeBaseLayer* scene();
    static void createBackground(CCLayer* target);
};


#endif //GDDLINTEGRATION_GDDLTHEMEBASELAYER_H