#include "Utils.h"

std::string Utils::floatToString(float number, int precision) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << number;
    return stream.str();
}

unsigned int Utils::getCurrentTimestamp() {
    const auto clockNow = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(clockNow.time_since_epoch()).count();
}

bool Utils::fileExists(const std::string& name) {
    struct stat buffer{};
    return stat(name.c_str(), &buffer) == 0;
}

bool Utils::fileIsEmpty(std::ifstream& pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();
}

void Utils::createTextInputNode(CCNode* parent, CCTextInputNode*& textfield, const std::string& font,
    const std::string& placeholder, int maxCharacters, const CCPoint& bgSize, const CCPoint& position, int zOrder) {
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
#ifdef GEODE_IS_MOBILE
        ++maxCharacters; // robert :)
#endif
    textfield->setMaxLabelLength(maxCharacters);
    textfield->setMaxLabelScale(0.7f);
}

void Utils::createGeodeTextInput(CCNode* parent, geode::TextInput*& textfield, const std::string& font,
    const std::string& placeholder, int maxCharacters, const CCPoint& bgSize, const CCPoint& position, int zOrder) {
    textfield = geode::TextInput::create(bgSize.x, placeholder.c_str(), font.c_str());
    parent->addChild(textfield, zOrder + 1);
    textfield->setPosition(position);
#ifdef GEODE_IS_MOBILE
    ++maxCharacters; // robert :)
#endif
    textfield->setMaxCharCount(maxCharacters);
}

void Utils::createLeftRightButtonsAround(CCNode* object, const CCPoint& size, CCObject* callbackObject,
    SEL_MenuHandler leftCallback, SEL_MenuHandler rightCallback, int zOrder) {
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

void Utils::createCheckbox(CCNode* parent, CCMenuItemToggler*& toggler, const std::string& label, float labelOffset,
    float scale, const CCPoint& position, CCObject* callbackObject, SEL_MenuHandler callback, int zOrder) {
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

int Utils::getNumberTextfieldValue(CCTextInputNode*& textfield) {
    return getNumberWithGivenDefaultTextfieldValue(textfield, 0);
}

int Utils::getNumberWithGivenDefaultTextfieldValue(CCTextInputNode*& textfield, int defaultValue) {
    if (textfield->getString().empty())
        return defaultValue;
    // instead of try/catch because of android
    int returnValue = defaultValue;
    auto returnValueResult = numFromString<int>(textfield->getString());
    if (returnValueResult.isOk()) {
        returnValue = returnValueResult.unwrap();
    }
    return returnValue;
}

void Utils::setNumberWithDefZeroTextfield(int value, CCTextInputNode*& textfield) {
    setNumberWithGivenDefaultValueTextfield(value, textfield, 0);
}

void Utils::setNumberWithGivenDefaultValueTextfield(int value, CCTextInputNode*& textfield, int defaultValue,
    std::string emptyPlaceholder) {
    if (value != defaultValue) {
        textfield->setString(std::to_string(value).c_str());
    } else {
        textfield->setString(emptyPlaceholder);
    }
}

void Utils::scaleLabelToWidth(CCLabelBMFont*& label, const float maxWidth) {
    const float scale =
        0.6f * label->getContentSize().width > maxWidth ? maxWidth / label->getContentSize().width : 0.6f;
    label->setScale(scale);
}

void Utils::createLabel(CCLayer* parent, const std::string& font, const std::string& text, float maxWidth,
    const CCPoint& position, int zOrder) {
    auto label = CCLabelBMFont::create(text.c_str(), font.c_str());
    parent->addChild(label, zOrder);
    label->setPosition(position);
    scaleLabelToWidth(label, maxWidth);
}

CCScale9Sprite* Utils::createLabelForChoice(CCLayer* parent, CCLabelBMFont*& label, const std::string& font,
    const std::string& placeholder, float maxWidth, const CCPoint& position, const CCPoint& bgSize, int zOrder) {
    label = CCLabelBMFont::create(placeholder.c_str(), font.c_str());
    parent->addChild(label, zOrder);
    label->setPosition(position);
    scaleLabelToWidth(label, maxWidth);
    const auto bg = CCScale9Sprite::create("square02b_001.png");
    parent->addChild(bg, zOrder + 1);
    bg->setContentSize(bgSize);
    bg->setScale(0.5f);
    bg->setContentSize(bg->getContentSize() / 0.5f);
    bg->setPosition(position);
    bg->setOpacity(90);
    bg->setColor({0, 0, 0});
    return bg;
}

std::function<void(web::WebResponse)> Utils::getCacheDownloadLambda(bool notifySuccess) {
    return [notifySuccess](web::WebResponse res) {
        const std::string response = res.string().unwrapOr("");
        if (response.empty()) {
            const std::string errorMessage = "GDDL Cache refresh failed - received empty response";
            Notification::create("GDDL Cache refresh failed - received empty response", NotificationIcon::Error, 3)->show();
            log::error("Utils::bindCacheDownloadCallback: {}", errorMessage);
        } else {
            if (res.code() == 200) {
                RatingsManager::cacheRatings(response);
                if (RatingsManager::cacheEmpty()) {
                    const std::string errorMessage = "GDDL Cache refresh failed - received no ratings";
                    Notification::create(errorMessage, NotificationIcon::Error, 3)->show();
                    log::error("Utils::bindCacheDownloadCallback: {}, raw response: {}", errorMessage, response);
                    // populate the cache from the save anyway, there could be something in there
                    RatingsManager::populateFromSave();
                    log::warn("Utils::bindCacheDownloadCallback: Reusing old cache...");
                } else  {
                    if (notifySuccess) {
                        Notification::create("GDDL Cache refresh succeded!", NotificationIcon::Success, 2)->show();
                    }
                    log::info("Utils::bindCacheDownloadCallback: GDDL Cache refresh succeded");
                }
            } else {
                const std::string errorMessage = "GDDL Cache refresh failed - " + getErrorMessageFromErrorCode(res.code()).value_or(res.string().unwrapOr("Response was not a valid string"));
                Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
                log::error("Utils::bindCacheDownloadCallback: [{}] {}", res.code(), errorMessage);
            }
        }
    };
}

CCSprite* Utils::getTierSpriteFromName(const char* name) {
    const auto sprite = CCSprite::create(Mod::get()->expandSpriteName(name).data());

    sprite->setScale(0.275f);
    sprite->setAnchorPoint({0, 0});

    return sprite;
}

CCSprite* Utils::getSpriteFromTier(const int tier) {
    if (tier == -1) {
        return getTierSpriteFromName("tier_unrated.png");
    }
    return getTierSpriteFromName(("tier_" + std::to_string(tier) + ".png").c_str());
}

bool Utils::notExcluded(int levelID) {
    const auto setting = static_pointer_cast<ExcludeRangeSettingV3>(Mod::get()->getSetting("exclude-range"));
    if (setting->getRangeBegin() == 0 && setting->getRangeEnd() == 0) return true;
    const int cachedTier = RatingsManager::getCachedTier(levelID);
    const int effectiveRangeEnd =
        setting->getRangeEnd() == 0 ? ExcludeRangeSettingV3::highestTier + 1 : setting->getRangeEnd();
    if (setting->isInclude()) {
        return cachedTier >= setting->getRangeBegin() && cachedTier <= effectiveRangeEnd;
    }
    return cachedTier < setting->getRangeBegin() || cachedTier > effectiveRangeEnd;
}

ccColor4F Utils::hexColorTo4F(int hexColor) {
    const float r = ((hexColor >> 16) & 0xFF) / 255.0;
    const float g = ((hexColor >> 8) & 0xFF) / 255.0;
    const float b = ((hexColor) & 0xFF) / 255.0;
    return ccc4f(r, g, b, 1.0f);
}

ccColor3B Utils::hexColorTo3B(int hexColor) {
    const int r = (hexColor >> (8 * 2)) & 0xff;
    const int g = (hexColor >> (8 * 1)) & 0xff;
    const int b = (hexColor >> (8*0)) & 0xff;
    return ccc3(r, g, b);
}

void Utils::recolorTextInLabel(CCLabelBMFont* label, std::string textToRecolor, int color, int searchOffset) {
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

int Utils::getFPS() {
    return std::round(1.0f / CCDirector::get()->getDeltaTime());
}

int Utils::getCorrectedFPS() {
    std::vector<int> commonFPSValues = {60, 75, 90, 120, 144, 165, 180, 240, 288, 300, 360};
    int readFPS = getFPS();
    for (const auto value: commonFPSValues) {
        if (std::abs(readFPS - value) <= 2) {
            readFPS = value;
            break;
        }
    }
    return readFPS;
}

bool Utils::isMobile() {
#ifdef GEODE_IS_MOBILE
        return true;
#else
    return false;
#endif
}

std::string Utils::getUserAgent() {
    return Mod::get()->getID() + "/" + Mod::get()->getVersion().toVString() + "; Geometry Dash/" + GEODE_GD_VERSION_STRING + "; Geode/" + Loader::get()->getVersion().toVString() + "; " + GEODE_PLATFORM_NAME;
}

std::string Utils::getGrayPopupBG() {
    const auto bgSetting = Mod::get()->getSettingValue<std::string>("popup-color");
    int color = 5; // gray
    if (bgSetting == "Brown") color = 1;
    else if (bgSetting == "Blue") color = 2;
    else if (bgSetting == "Green") color = 3;
    else if (bgSetting == "Purple") color = 4;
    return "GJ_square0" + std::to_string(color) + ".png";
}

CCSprite* Utils::getGrayPopupCloseButton(const float scale) {
    const auto bgSetting = Mod::get()->getSettingValue<std::string>("popup-color");
    if (bgSetting == "Brown") {
        const auto sprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        sprite->setScale(scale / 0.85f);
        return sprite;
    }
    auto color = CircleBaseColor::Gray;
    if (bgSetting == "Blue") color = CircleBaseColor::Blue;
    else if (bgSetting == "Green") color = CircleBaseColor::Green;
    else if (bgSetting == "Purple") color = CircleBaseColor::DarkPurple;
    return CircleButtonSprite::createWithSpriteFrameName("geode.loader/close.png", scale, color);
}

std::optional<std::string> Utils::getErrorMessageFromErrorCode(int errorCode) {
    if (errorCode == 401) {
        return "Unauthorized";
    }
    if (errorCode == 403) {
        return "Forbidden";
    }
    if (errorCode == 408 || errorCode == 522) {
        return "Timed out";
    }
    if (errorCode == 413) {
        return "Payload too large";
    }
    if (errorCode == 414) {
        return "Request URI too long";
    }
    if (errorCode == 418) {
        return "I'm a teapot";
    }
    if (errorCode == 420) {
        return "Enhance your calm";
    }
    if (errorCode == 429) {
        return "Too many requests";
    }
    if (errorCode == 444) {
        return "No response";
    }
    if (errorCode == 495 || errorCode == 496) {
        return "SSL certificate error";
    }
    if (errorCode == 500) {
        return "Internal server error";
    }
    if (errorCode == 502) {
        return "Bad gateway";
    }
    if (errorCode == 503) {
        return "Service unavailable";
    }
    if (errorCode == 504) {
        return "Gateway timeout";
    }
    return std::nullopt;
}

std::string Utils::getErrorFromMessageAndResponse(matjson::Value jsonResponse, web::WebResponse& res) {
    std::string errorMessage = getErrorMessageFromErrorCode(res.code()).value_or(res.string().unwrapOr(std::to_string(res.code())));
    if (jsonResponse.contains("message")) {
        if (jsonResponse["message"].isArray()) {
            errorMessage = jsonResponse["message"].asArray().unwrap()[0].asString().unwrapOr("Response was not a valid string");
        } else if (jsonResponse["message"].isString()) {
            errorMessage = jsonResponse["message"].asString().unwrapOr("Response was not a valid string");
        }
    }
    return errorMessage;
}

void Utils::addAuthHeader(web::WebRequest &req) {
    if (const std::string sid = Mod::get()->getSavedValue<std::string>("login-sid", ""); !sid.empty()) {
        req.header("Cookie", fmt::format("gddl.sid={}", sid));
    } else {
        req.header("Authorization", fmt::format("Bearer {}", Mod::get()->getSavedValue<std::string>("api-key", "")));
    }
}

// thank you @undefined06855!
Result<std::string_view> Utils::getSpriteNodeFrameName(CCSprite* sprite) {
    std::string_view frameName = "";
    // taken from devtools
    if (auto texture = sprite->getTexture()) {
        auto cachedFrames = CCSpriteFrameCache::sharedSpriteFrameCache()->m_pSpriteFrames;
        auto rect = sprite->getTextureRect();
        for (auto [key, frame] : geode::cocos::CCDictionaryExt<std::string_view, CCSpriteFrame*>(cachedFrames)) {
            if (frame->getTexture() == texture && frame->getRect() == rect) {
                frameName = key;
                break;
            }
        }
    }

    if (frameName.empty()) {
        return Err("Sprite does not have a frame name found in cache!");
    }

    return Ok(frameName);
}
