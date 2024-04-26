#ifndef GDDL_UTILS_H
#define GDDL_UTILS_H
#include <chrono>
#include <string>
#include <sys/stat.h>

class Utils {
public:
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
        struct stat buffer {};
        return stat(name.c_str(), &buffer) == 0;
    }

    // https://stackoverflow.com/questions/2390912/checking-for-an-empty-file-in-c
    static bool fileIsEmpty(std::ifstream& pFile)
    {
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

    static void createTextInputNode(CCLayer *parent, CCTextInputNode *&textfield, const std::string &font,
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

    static void createLeftRightButtonsAround(CCNode *object, const CCPoint &size, CCObject *callbackObject, SEL_MenuHandler leftCallback,
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
};
#endif // GDDL_UTILS_H
