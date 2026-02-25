#ifndef GDDLINTEGRATION_SEARCHINPUTCONTROL_H
#define GDDLINTEGRATION_SEARCHINPUTCONTROL_H

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SearchInputControl : public CCNode {
protected:
    static constexpr CCPoint controlSize = {145.0f, 60.0f};
    static constexpr float standardComponentWidth = 110.0f;
    static constexpr float standardComponentHeight = 30.0f;

    CCMenu* controlMenu = nullptr;

    bool init() override;
public:
    static SearchInputControl *create();
    void relocateToGlobalMenu(CCMenu* popupMainMenu);
    
    virtual void saveSetting() {};
    virtual void loadSetting() {};
};


#endif //GDDLINTEGRATION_SEARCHINPUTCONTROL_H