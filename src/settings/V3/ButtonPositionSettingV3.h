#ifndef BUTTONPOSITIONSETTINGV3_H
#define BUTTONPOSITIONSETTINGV3_H

#include <Geode/loader/SettingV3.hpp>

enum ButtonPosition {
    DEFAULT,
    TO_THE_LEFT_OF_THE_LEVEL_TITLE,
    TO_THE_RIGHT_OF_THE_LEVEL_TITLE,
    NUM_VALUES,
};

using namespace geode::prelude;

class ButtonPositionSettingV3 final : public SettingV3 {
protected:
    ButtonPosition position{};

public:
    static constexpr ButtonPosition defaultPosition = DEFAULT;

    static Result<std::shared_ptr<ButtonPositionSettingV3>> parse(std::string const& key, std::string const& modID,
                                                                  matjson::Value const& json);
    bool load(const matjson::Value& json) override;
    bool save(matjson::Value& json) const override;
    SettingNodeV3* createNode(float width) override;
    bool isDefaultValue() const override;
    void reset() override;

    ButtonPosition getPosition() const;
    void setPosition(ButtonPosition position);
};

$execute {
    (void)Mod::get()->registerCustomSettingType("button-position-v3", &ButtonPositionSettingV3::parse);
}

#endif //BUTTONPOSITIONSETTINGV3_H
