#ifndef GDDLINTEGRATION_GDDLADVANCEDLEVELINFOPOPUP_H
#define GDDLINTEGRATION_GDDLADVANCEDLEVELINFOPOPUP_H

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class GDDLAdvancedLevelInfoPopup final : public FLAlertLayer {
    EventListener<web::WebTask> spreadListener;
    int levelID;

    CCMenuItemSpriteExtra* m_closeBtn{};

    bool init(const int levelID);
    void onClose(cocos2d::CCObject* sender);

    void prepareSearchListener();
    void addBarCharts();
    static std::string getSpreadEndpointUrl(const int levelID);

public:
    static GDDLAdvancedLevelInfoPopup* create(const int levelID);
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLADVANCEDLEVELINFOPOPUP_H
