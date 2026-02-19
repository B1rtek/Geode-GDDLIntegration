#ifndef GDDLINTEGRATION_SEARCHLAYERPAGE_H
#define GDDLINTEGRATION_SEARCHLAYERPAGE_H

#include <Geode/Geode.hpp>

#include "searchcontrols/SearchInputControl.h"

class SearchLayerPage : public cocos2d::CCNode {
    static constexpr CCPoint pageSize = {435.0f, 180.0f};

    std::vector<SearchInputControl*> controls;

    bool init() override;
public:
    static SearchLayerPage *create();

    void addControl(SearchInputControl* control, CCMenu* targetGlobalMenu = nullptr);
    void clearContent();
    void saveSettings();

    static CCPoint getControlPosition(unsigned index);
};


#endif //GDDLINTEGRATION_SEARCHLAYERPAGE_H