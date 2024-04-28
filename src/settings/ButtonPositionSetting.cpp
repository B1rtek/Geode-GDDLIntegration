#include "ButtonPositionSetting.h"

#include "ButtonPositionSettingNode.h"

bool ButtonPositionSetting::load(matjson::Value const &json) {
    if (!json.contains("button-position")) {
        return false;
    }
    position = static_cast<ButtonPosition>(json["button-position"].as_int());
    return true;
}

bool ButtonPositionSetting::save(matjson::Value &json) const {
    json = matjson::Object {
        {"button-position", static_cast<int>(position)}
    };
    return true;
}

SettingNode * ButtonPositionSetting::createNode(float width) {
    // return ButtonPositionSettingNode::create(this, width);
}

ButtonPosition ButtonPositionSetting::getPosition() const {
    return position;
}

void ButtonPositionSetting::setPosition(ButtonPosition position) {
    this->position = position;
}
