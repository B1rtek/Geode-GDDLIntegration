#ifndef GDDLINTEGRATION_GDDLLEVELINFOPOPUP_H
#define GDDLINTEGRATION_GDDLLEVELINFOPOPUP_H

#include <Geode/Bindings.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class GDDLLevelInfoPopup final : public FLAlertLayer {
    EventListener<web::WebTask> ratingListener;
    int levelID;
    const inline static CCPoint popupSize = {300.0f, 160.0f};

    explicit GDDLLevelInfoPopup(int levelId);
    bool init() override;
    void onClose(CCObject *sender);
    void onOpenInBrowser(CCObject *sender);

    void prepareSearchListener();
    void addLevelInfo();
public:
    static GDDLLevelInfoPopup* create(int levelID);
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLLEVELINFOPOPUP_H

