#ifndef GDDLINTEGRATION_GDDLADVANCEDLEVELINFOPOPUP_H
#define GDDLINTEGRATION_GDDLADVANCEDLEVELINFOPOPUP_H

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class GDDLAdvancedLevelInfoPopup final : public FLAlertLayer {
    async::TaskHolder<web::WebResponse> ratingListener, spreadListener, skillsetsListener;
    int gddlLevelID = 0;
    std::string levelName, creator;
    GJGameLevel* level = nullptr;
    const CCPoint popupSize = {440.0f, 280.0f};

    CCMenuItemSpriteExtra* m_closeBtn{};
    CCLabelBMFont* levelNameLabel = nullptr;

    bool init(GJGameLevel* level, int gddlLevelID);
    void onClose(cocos2d::CCObject* sender);
    void onSkillsetClicked(CCObject* sender);
    void onSkillsetInfo(CCObject* sender);
    void onSubmitClicked(CCObject* sender);
    void onShowcaseClicked(CCObject* sender);
    void onOpenInBrowserClicked(CCObject* sender);

    std::function<void(web::WebResponse)> getRatingListenerLambda();
    std::function<void(web::WebResponse)> getSpreadListenerLambda();
    std::function<void(web::WebResponse)> getSkillsetsListenerLambda();
    void addBarCharts();
    void addSkillsets();
    void addShowcaseButton(bool active);
    void addTierSprite(int tier);
    static std::string getSpreadEndpointUrl(const int levelID);
    static std::string getSkillsetsEndpointUrl(const int levelID);

public:
    static GDDLAdvancedLevelInfoPopup* create(GJGameLevel* level, int gddlLevelID);
    void show() override;

    void addRatingInfo();
};


#endif //GDDLINTEGRATION_GDDLADVANCEDLEVELINFOPOPUP_H
