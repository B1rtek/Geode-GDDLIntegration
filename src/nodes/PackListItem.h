#ifndef GDDLINTEGRATION_PACKLISTITEM_H
#define GDDLINTEGRATION_PACKLISTITEM_H

#include <Geode/Bindings.hpp>
#include <objects/PackInfo.h>

using namespace geode::prelude;

class PackListItem : public CCNode {
    static constexpr float itemHeight = 50.0f;
    const inline static std::string packIconsBaseUrl = "https://gdladder.com/packIcons/";

    bool init(const float width, const PackInfo& packInfo);

    void onView(CCObject* sender);
public:
    static PackListItem* create(const float width, const PackInfo& packInfo);
};


#endif //GDDLINTEGRATION_PACKLISTITEM_H