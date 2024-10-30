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
public:
    const inline static std::string hopefullyAllCharactersAnyoneWillEverNeed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+`~[]{}/?.>,<\\|;:'\" ";

    static std::string floatToString(float number, int precision) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << number;
        return stream.str();
    }

    static unsigned int getCurrentTimestamp() {
        const auto clockNow = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(clockNow.time_since_epoch()).count();
    }

    // I love stack overflow <3
    // https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
    static bool fileExists(const std::string &name) {
        struct stat buffer{};
        return stat(name.c_str(), &buffer) == 0;
    }

    // https://stackoverflow.com/questions/2390912/checking-for-an-empty-file-in-c
    static bool fileIsEmpty(std::ifstream &pFile) {
        return pFile.peek() == std::ifstream::traits_type::eof();
    }

    template<typename T>
    static std::vector<T> copySetToVector(std::set<T> setToCopy) {
        std::vector<T> vec;
        for (auto element: setToCopy) {
            vec.push_back(element);
        }
        return vec;
    }

    template<typename T>
    static std::set<T> copyVectorToSet(std::vector<T> vectorToCopy) {
        std::set<T> set;
        for (auto element: vectorToCopy) {
            set.insert(element);
        }
        return set;
    }

    static void createTextInputNode(CCNode *parent, CCTextInputNode *&textfield, const std::string &font,
                                    const std::string &placeholder, int maxCharacters, const CCPoint &bgSize,
                                    const CCPoint &position, int zOrder = 1) {
        const auto bg = CCScale9Sprite::create("square02_small.png");
        parent->addChild(bg, zOrder);
        bg->setContentSize(bgSize);
        bg->setScale(0.5f);
        bg->setContentSize(bg->getContentSize() / 0.5f);
        bg->setPosition(position);
        bg->setOpacity(100);
        textfield = CCTextInputNode::create(bgSize.x, bgSize.y, placeholder.c_str(), font.c_str());
        parent->addChild(textfield, zOrder + 1);
        textfield->setPosition(position);
#ifdef GEODE_IS_ANDROID
        ++maxCharacters; // robert :)
#endif
        textfield->setMaxLabelLength(maxCharacters);
        textfield->setMaxLabelScale(0.7f);
    }

    static void createLeftRightButtonsAround(CCNode *object, const CCPoint &size, CCObject *callbackObject,
                                             SEL_MenuHandler leftCallback,
                                             SEL_MenuHandler rightCallback, int zOrder = 1) {
        // left
        const CCPoint positionLeft =
                object->getPosition() -
                CCPoint(object->getContentSize().width * object->getScale() / 2 + size.x / 2 + 0.5f, -1.0f);
        const auto leftButtonSprite = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
        leftButtonSprite->setScale(0.8f);
        const auto leftButton = CCMenuItemSpriteExtra::create(leftButtonSprite, callbackObject, leftCallback);
        object->getParent()->addChild(leftButton, zOrder);
        leftButton->setPosition(positionLeft);
        leftButton->setContentSize(size);
        // right
        const CCPoint positionRight = positionLeft + CCPoint(object->getContentSize().width * object->getScale() +
                                                             leftButton->getContentSize().width + 2.5f,
                                                             0.0f); // why is this not symmetrical wtf
        const auto rightButtonSprite = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
        rightButtonSprite->setScale(0.8f);
        const auto rightButton = CCMenuItemSpriteExtra::create(rightButtonSprite, callbackObject, rightCallback);
        object->getParent()->addChild(rightButton, zOrder);
        rightButton->setPosition(positionRight);
        rightButton->setContentSize(size);
    }

    static void createCheckbox(CCNode *parent, CCMenuItemToggler *&toggler, const std::string &label, float labelOffset,
                               float scale, const CCPoint &position, CCObject *callbackObject, SEL_MenuHandler callback,
                               int zOrder = 1) {
        toggler = CCMenuItemToggler::createWithStandardSprites(callbackObject, callback, scale);
        parent->addChild(toggler, zOrder);
        toggler->setPosition(position);
        const auto toggleLabel = CCLabelBMFont::create(label.c_str(), "bigFont.fnt");
        parent->addChild(toggleLabel, zOrder);
        toggleLabel->setPosition({toggler->getPositionX(), toggler->getPositionY() - labelOffset});
        const float maxWidth = toggler->getContentSize().width * scale * 2.0f;
        const float labelScale = 0.3f * toggleLabel->getContentSize().width > maxWidth
                                 ? maxWidth / toggleLabel->getContentSize().width
                                 : 0.3f;
        toggleLabel->setScale(labelScale);
    }

    static int getNumberTextfieldValue(CCTextInputNode *&textfield) {
        if (textfield->getString().empty())
            return 0;
        // instead of try/catch because of android
        int returnValue = 0;
        auto returnValueResult = numFromString<int>(textfield->getString());
        if (returnValueResult.isOk()) {
            returnValue = returnValueResult.value();
        }
        return returnValue;
    }

    static void setNumberWithDefZeroTextfield(int value, CCTextInputNode *&textfield) {
        if (value != 0) {
            textfield->setString(std::to_string(value).c_str());
        } else {
            textfield->setString("");
        }
    }

    static void scaleLabelToWidth(CCLabelBMFont *&label, const float maxWidth) {
        const float scale =
                0.6f * label->getContentSize().width > maxWidth ? maxWidth / label->getContentSize().width : 0.6f;
        label->setScale(scale);
    }

    static void createLabel(CCLayer *parent, const std::string &font, const std::string &text, float maxWidth,
                            const CCPoint &position, int zOrder = 1) {
        auto label = CCLabelBMFont::create(text.c_str(), font.c_str());
        parent->addChild(label, zOrder);
        label->setPosition(position);
        scaleLabelToWidth(label, maxWidth);
    }

    static CCScale9Sprite *createLabelForChoice(CCLayer *parent, CCLabelBMFont *&label, const std::string &font,
                                                const std::string &placeholder, float maxWidth, const CCPoint &position,
                                                const CCPoint &bgSize, int zOrder = 1) {
        label = CCLabelBMFont::create(placeholder.c_str(), font.c_str());
        parent->addChild(label, zOrder);
        label->setPosition(position);
        scaleLabelToWidth(label, maxWidth);
        const auto bg = CCScale9Sprite::create("square02_small.png");
        parent->addChild(bg, zOrder + 1);
        bg->setContentSize(bgSize);
        bg->setScale(0.5f);
        bg->setContentSize(bg->getContentSize() / 0.5f);
        bg->setPosition(position);
        bg->setOpacity(100);
        return bg;
    }

    static void bindCacheDownloadCallback(EventListener<web::WebTask> &cacheEventListener, bool notifySuccess = false) {
        cacheEventListener.bind([notifySuccess](web::WebTask::Event *e) {
            if (web::WebResponse * res = e->getValue()) {
                const std::string response = res->string().unwrapOr("");
                if (response.empty()) {
                    FLAlertLayer::create("GDDL Integration",
                                         "Failed to cache ratings from gdladder.com, check your internet connection.",
                                         "OK")->show();
                } else {
                    RatingsManager::cacheRatings(response);
                    if (!RatingsManager::cacheNotEmpty()) {
                        FLAlertLayer::create("GDDL Integration",
                                             "Failed to cache ratings from gdladder.com, check your internet connection.",
                                             "OK")->show();
                        // populate the cache from the save anyway, there could be something in there
                        RatingsManager::populateFromSave();
                    } else if (notifySuccess) {
                        FLAlertLayer::create("Reset GDDL cache", "Cache refresh succeeded", "OK")->show();
                    }
                }
            } else if (e->isCancelled()) {
                FLAlertLayer::create("GDDL Integration",
                                     "Failed to cache ratings from gdladder.com, check your internet connection.",
                                     "OK")->show();
            }
        });
    }

    static CCSprite *getTierSpriteFromName(const char *name) {
        const auto sprite = CCSprite::create(Mod::get()->expandSpriteName(name).data());

        sprite->setScale(0.275f);
        sprite->setAnchorPoint({0, 0});

        return sprite;
    }

    static CCSprite *getSpriteFromTier(const int tier) {
        if (tier == -1) {
            return getTierSpriteFromName("tier_unrated.png");
        }
        return getTierSpriteFromName(("tier_" + std::to_string(tier) + ".png").c_str());
    }

    static bool notExcluded(int levelID) {
        const auto setting = static_pointer_cast<ExcludeRangeSettingV3>(Mod::get()->getSettingV3("exclude-range"));
        if (setting->getRangeBegin() == 0 && setting->getRangeEnd() == 0) return true;
        const int cachedTier = RatingsManager::getCachedTier(levelID);
        const int effectiveRangeEnd =
                setting->getRangeEnd() == 0 ? ExcludeRangeSettingV3::highestTier + 1 : setting->getRangeEnd();
        if (setting->isInclude()) {
            return cachedTier >= setting->getRangeBegin() && cachedTier <= effectiveRangeEnd;
        }
        return cachedTier < setting->getRangeBegin() || cachedTier > effectiveRangeEnd;
    }

    static ccColor4F hexColorTo4F(int hexColor) {
        const float r = ((hexColor >> 16) & 0xFF) / 255.0;
        const float g = ((hexColor >> 8) & 0xFF) / 255.0;
        const float b = ((hexColor) & 0xFF) / 255.0;
        return ccc4f(r, g, b, 1.0f);
    }

    static ccColor3B hexColorTo3B(int hexColor) {
        const int r = (hexColor >> (8 * 2)) & 0xff;
        const int g = (hexColor >> (8 * 1)) & 0xff;
        const int b = (hexColor >> (8*0)) & 0xff;
        return ccc3(r, g, b);
    }

    static void recolorTextInLabel(CCLabelBMFont *label, std::string textToRecolor, int color, int searchOffset = 0) {
        const std::string labelText = label->getString();
        const int beginIndex = labelText.find(textToRecolor, searchOffset);
        if (beginIndex == std::string::npos) return;
        // calculate length without spaces
        int fontSpriteLength = 0;
        std::for_each(textToRecolor.begin(), textToRecolor.end(),
                      [&fontSpriteLength](char letter) { if (letter != ' ') ++fontSpriteLength; });
        // recolor
        for (int i = beginIndex; i < beginIndex + fontSpriteLength; i++) {
            typeinfo_cast<CCNodeRGBA*>(label->getChildren()->objectAtIndex(i))->setColor(hexColorTo3B(color));
        }
    }
};

#endif // GDDL_UTILS_H
