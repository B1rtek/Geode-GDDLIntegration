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
    matjson::Value submissionJson = matjson::Value();
    std::string requestedUsername;

    const inline static std::vector<std::string> device = {"PC", "Mobile"};
    int rating = -1, enjoyment = -1, fps = 0, gddlLevelID = 0, percent = 0, attempts = 0;
    bool mobile = false, twoPlayer = false, soloCompletion = true;
    const inline static std::vector<std::string> validProofURLs = {
        "https://www.youtube.com/watch?v=",
        "https://m.youtube.com/watch?v=",
        "https://youtube.com/watch?v=",
        "https://youtu.be/",
        "https://www.twitch.tv/videos/",
        "https://twitch.tv/videos/",
        "https://drive.google.com/file/d/",
        "https://www.bilibili.com/video/",
        "https://m.bilibili.com/video/",
        "https://bilibili.com/video/"
    };


    bool init(GJGameLevel* level, int gddlLevelID);
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

    CCLabelBMFont* addLabel(const std::string& text, const CCPoint& position, float scale = 0.7f,
                            std::string font = "chatFont.fnt");
    void addInfoButtonAndCenterLabel(CCLabelBMFont* label, CCSprite* iButtonSprite, SEL_MenuHandler callback, const float centerAroundX);
    void setInitialValues();
    void updateTextfields();
    void prepareSubmissionListeners();
    bool isValidProof(const std::string& proofURL);
    std::string fillOutSubmissionJson();
    void makeSubmissionRequest();

public:
    static GDDLRatingSubmissionLayer* create(GJGameLevel* level, int gddlLevelID);
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLRATINGSUBMISSIONLAYER_H
