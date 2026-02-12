#ifndef GDDL_UTILS_H
#define GDDL_UTILS_H

#include <chrono>
#include <string>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/utils/web.hpp>
#include <sys/stat.h>

#include "RatingsManager.h"
#include "settings/ExcludeRangeSettingV3.h"

class Utils {
    const inline static std::vector<std::string> bgOptions = {
        "Brown", "Blue", "Green", "Purple", "Gray", "White", "Transparent???"
    };
public:
    const inline static std::string hopefullyAllCharactersAnyoneWillEverNeed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+`~[]{}/?.>,<\\|;:'\" ";

    static std::string floatToString(float number, int precision);
    static unsigned int getCurrentTimestamp();
    // I love stack overflow <3
    // https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
    static bool fileExists(const std::string &name);
    // https://stackoverflow.com/questions/2390912/checking-for-an-empty-file-in-c
    static bool fileIsEmpty(std::ifstream &pFile);

    template<typename T>
    static std::vector<T> copySetToVector(std::set<T> setToCopy);
    template<typename T>
    static std::set<T> copyVectorToSet(std::vector<T> vectorToCopy);

    static void createTextInputNode(CCNode *parent, CCTextInputNode *&textfield, const std::string &font,
                                    const std::string &placeholder, int maxCharacters, const CCPoint &bgSize,
                                    const CCPoint &position, int zOrder = 1);
    static void createGeodeTextInput(CCNode *parent, geode::TextInput *&textfield, const std::string &font,
                                    const std::string &placeholder, int maxCharacters, const CCPoint &bgSize,
                                    const CCPoint &position, int zOrder = 1);
    static void createLeftRightButtonsAround(CCNode *object, const CCPoint &size, CCObject *callbackObject,
                                             SEL_MenuHandler leftCallback,
                                             SEL_MenuHandler rightCallback, int zOrder = 1);
    static void createCheckbox(CCNode *parent, CCMenuItemToggler *&toggler, const std::string &label, float labelOffset,
                               float scale, const CCPoint &position, CCObject *callbackObject, SEL_MenuHandler callback,
                               int zOrder = 1);
    static int getNumberTextfieldValue(CCTextInputNode *&textfield);
    static int getNumberWithGivenDefaultTextfieldValue(CCTextInputNode *&textfield, int defaultValue);
    static void setNumberWithDefZeroTextfield(int value, CCTextInputNode *&textfield);
    static void setNumberWithGivenDefaultValueTextfield(int value, CCTextInputNode *&textfield, int defaultValue, std::string emptyPlaceholder = "");
    static void scaleLabelToWidth(CCLabelBMFont *&label, const float maxWidth);
    static void createLabel(CCLayer *parent, const std::string &font, const std::string &text, float maxWidth,
                            const CCPoint &position, int zOrder = 1);
    static CCScale9Sprite *createLabelForChoice(CCLayer *parent, CCLabelBMFont *&label, const std::string &font,
                                                const std::string &placeholder, float maxWidth, const CCPoint &position,
                                                const CCPoint &bgSize, int zOrder = 1);
    static std::function<void(web::WebResponse)> getCacheDownloadLambda(bool notifySuccess = false);
    static CCSprite *getTierSpriteFromName(const char *name);
    static CCSprite *getSpriteFromTier(const int tier);
    static bool notExcluded(int levelID);
    static ccColor4F hexColorTo4F(int hexColor);
    static ccColor3B hexColorTo3B(int hexColor);
    static void recolorTextInLabel(CCLabelBMFont *label, std::string textToRecolor, int color, int searchOffset = 0);
    static int getFPS();
    static int getCorrectedFPS();
    static bool isMobile();
    static std::string getUserAgent();
    static std::string getGrayPopupBG();
    static CCSprite* getGrayPopupCloseButton(float scale = .85f);
    static std::optional<std::string> getErrorMessageFromErrorCode(int errorCode);
    static std::string getErrorFromMessageAndResponse(matjson::Value jsonResponse, web::WebResponse& res);
    static void addAuthHeader(web::WebRequest &req);
    static Result<std::string_view> getSpriteNodeFrameName(CCSprite* sprite);
};

template <typename T>
std::vector<T> Utils::copySetToVector(std::set<T> setToCopy) {
    std::vector<T> vec;
    for (auto element: setToCopy) {
        vec.push_back(element);
    }
    return vec;
}

template <typename T>
std::set<T> Utils::copyVectorToSet(std::vector<T> vectorToCopy) {
    std::set<T> set;
    for (auto element: vectorToCopy) {
        set.insert(element);
    }
    return set;
}

#endif // GDDL_UTILS_H
