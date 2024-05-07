#ifndef BUTTONPOSITIONSETTING_H
#define BUTTONPOSITIONSETTING_H

#include <string>
#include <Geode/Prelude.hpp>
#include <Geode/loader/Setting.hpp>

enum ButtonPosition {
    DEFAULT,
    TO_THE_LEFT_OF_THE_LEVEL_TITLE,
    TO_THE_RIGHT_OF_THE_LEVEL_TITLE,
    DEMON_FACE
};

using namespace geode::prelude;

class ButtonPositionSetting : public SettingValue {
protected:
    ButtonPosition position;

public:
    ButtonPositionSetting(std::string const &key, std::string const &modID, ButtonPosition position) : SettingValue(key, modID), position(position) {}

    bool load(matjson::Value const& json) override;

    bool save(matjson::Value& json) const override;

    SettingNode* createNode(float width) override;

    ButtonPosition getPosition() const;

    void setPosition(ButtonPosition position);
};



#endif //BUTTONPOSITIONSETTING_H
