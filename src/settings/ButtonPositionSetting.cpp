#include "ButtonPositionSetting.h"

#include "ButtonPositionSettingNode.h"

bool ButtonPositionSetting::load(matjson::Value const &json) {
    if (!json.contains("button-position")) {
        return false;
    }
    int positionNumber = json["button-position"].is_number() ? json["button-position"].as_int() : 0;
    // in case it's set to something stupid
    positionNumber = std::min(static_cast<int>(ButtonPositionSettingNode::toDisplay.size()), std::max(0, positionNumber));
    position = static_cast<ButtonPosition>(positionNumber);

    return true;
}

bool ButtonPositionSetting::save(matjson::Value &json) const {
    json = matjson::Object {
        {"button-position", static_cast<int>(position)}
    };
    return true;
}

SettingNode * ButtonPositionSetting::createNode(float width) {
    return ButtonPositionSettingNode::create(this, width);
}

ButtonPosition ButtonPositionSetting::getPosition() const {
    return position;
}

void ButtonPositionSetting::setPosition(ButtonPosition position) {
    this->position = position;
}
