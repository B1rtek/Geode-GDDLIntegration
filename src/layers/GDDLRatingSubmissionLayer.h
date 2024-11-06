#ifndef GDDLINTEGRATION_GDDLRATINGSUBMISSIONLAYER_H
#define GDDLINTEGRATION_GDDLRATINGSUBMISSIONLAYER_H

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

/**
 * json schema
 * {
 *  "levelID":341613,
 *  "rating":2,
 *  "enjoyment":6,
 *  "refreshRate":60,
 *  "device":1, - 1 for pc, 2 for mobile
 *  "proof":"https://youtu.be/3-BUEoH9WBs",
 *  "progress":100,
 *  "attempts":403,
 *  "isSolo":true - default for both 1p and 2p levels
 *  "secondPlayerID": int | null
 *  }
 *
 *  headers:
 *  Cookie: gddl.sid.sig=<sid.sig>; gddl.sid=<sid>
 */

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

    const inline static std::string submissionEndpoint = "https://gdladder.com/api/login";
    const inline static std::string userSearchEndpoint = "https://gdladder.com/api/user/search";

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

public:
    static GDDLRatingSubmissionLayer* create(GJGameLevel* level);
    void show() override;
};


#endif //GDDLINTEGRATION_GDDLRATINGSUBMISSIONLAYER_H
