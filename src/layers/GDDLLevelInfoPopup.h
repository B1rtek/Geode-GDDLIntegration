#ifndef GDDLINTEGRATION_GDDLLEVELINFOPOPUP_H
#define GDDLINTEGRATION_GDDLLEVELINFOPOPUP_H

#include <Geode/Bindings.hpp>
#include <Geode/utils/web.hpp>


using namespace geode::prelude;

class GDDLLevelInfoPopup final : public FLAlertLayer {
    EventListener<web::WebTask> spreadListener;
    int levelID;
    static constexpr CCPoint popupSize = {500.0f, 340.0f};

    explicit GDDLLevelInfoPopup(int levelId);
    bool init() override;
    void onClose(CCObject *sender);
    void onOpenInBrowser(CCObject *sender);

    void prepareSpreadListener();
    void addBarCharts();
    void addLevelInfo();
    static std::string getSpreadEndpointUrl(const int levelID);
public:
    static GDDLLevelInfoPopup* create(int levelID);
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLLEVELINFOPOPUP_H
