#ifndef GDDLINTEGRATION_GDDLRATINGSUBMISSIONLAYER_H
#define GDDLINTEGRATION_GDDLRATINGSUBMISSIONLAYER_H

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class GDDLRatingSubmissionLayer final : public FLAlertLayer {
    CCMenuItemSpriteExtra* m_closeBtn{};

    CCTextInputNode* ratingTextfield = nullptr;
    CCTextInputNode* enjoymentTextfield = nullptr;
    CCTextInputNode* fpsTextfield = nullptr;
    CCTextInputNode* proofTextfield = nullptr;
    CCTextInputNode* percentTextfield = nullptr;
    CCTextInputNode* attemptsTextfield = nullptr;
    CCTextInputNode* secondPlayerTextfield = nullptr;
    CCLabelBMFont* deviceLabel = nullptr;
    CCMenuItemToggler* soloCompletionToggler = nullptr;

    const inline static std::string submissionEndpoint = "https://gdladder.com/api/submit";
    const inline static std::string userSearchEndpoint = "https://gdladder.com/api/user/search";
    EventListener<web::WebTask> submissionListener, userSearchListener;

    const inline static std::vector<std::string> device = {"PC", "Mobile"};
    int rating = -1, enjoyment = -1, fps = 0, levelID = 0, percent = 0, attempts = 0;
    bool mobile = false, twoPlayer = false, soloCompletion = true;

    bool init(GJGameLevel* level);
    void onClose(CCObject* sender);

    void onRatingLeft(CCObject* sender);
    void onRatingRight(CCObject* sender);
    void onEnjoymentLeft(CCObject* sender);
    void onEnjoymentRight(CCObject* sender);
    void onFPSLeft(CCObject* sender);
    void onFPSRight(CCObject* sender);
    void onDeviceRight(CCObject* sender);
    void onDeviceLeft(CCObject* sender);
    void onToggleSoloCompletion(CCObject* sender);
    void onSubmitClicked(CCObject* sender);
    void onGuidelinesClicked(CCObject* sender);
    void onRatingInfo(CCObject* sender);
    void onEnjoymentInfo(CCObject* sender);
    void onFPSInfo(CCObject* sender);
    void onPercentInfo(CCObject* sender);
    void onAttemptsInfo(CCObject* sender);
    void onProofInfo(CCObject* sender);
    void onSecondPlayerInfo(CCObject* sender);

    CCLabelBMFont * addLabel(const std::string& text, const CCPoint& position, float scale = 0.7f, std::string font = "chatFont.fnt");
    void addInfoButton(CCLabelBMFont *label, CCSprite *iButtonSprite, SEL_MenuHandler callback);
    void setInitialValues();
    void updateTextfields();
    void prepareSubmissionListeners();

public:
    static GDDLRatingSubmissionLayer* create(GJGameLevel* level);
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLRATINGSUBMISSIONLAYER_H
