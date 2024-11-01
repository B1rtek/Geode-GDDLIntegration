#ifndef GDDLINTEGRATION_GDDLADVANCEDLEVELINFOPOPUP_H
#define GDDLINTEGRATION_GDDLADVANCEDLEVELINFOPOPUP_H

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class GDDLAdvancedLevelInfoPopup final : public FLAlertLayer {
    EventListener<web::WebTask> spreadListener, skillsetsListener;
    int levelID = 0;
    std::string levelName, creator;
    const CCPoint popupSize = {440.0f, 280.0f};

    CCMenuItemSpriteExtra* m_closeBtn{};
    CCLabelBMFont* levelNameLabel = nullptr;

    bool init(const int levelID, const std::string& levelName, const std::string& creator);
    void onClose(cocos2d::CCObject* sender);
    void onSkillsetClicked(CCObject* sender);

    void prepareSearchListeners();
    void addBarCharts();
    void addSkillsets();
    static std::string getSpreadEndpointUrl(const int levelID);
    static std::string getSkillsetsEndpointUrl(const int levelID);

public:
    static GDDLAdvancedLevelInfoPopup* create(const int levelID, const std::string& levelName, const std::string& creator);
    void show() override;

    void addRatingInfo();
};


#endif //GDDLINTEGRATION_GDDLADVANCEDLEVELINFOPOPUP_H
