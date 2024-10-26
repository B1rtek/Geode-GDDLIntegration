#include "ButtonPositionSettingV3.h"
#include "ButtonPositionSettingNodeV3.h"

Result<std::shared_ptr<ButtonPositionSettingV3>> ButtonPositionSettingV3::parse(std::string const& key,
    std::string const& modID, matjson::Value const& json) {
    const auto res = std::make_shared<ButtonPositionSettingV3>();
    auto root = checkJson(json, "ButtonPositionSettingV3");

    res->init(key, modID, root);
    res->parseNameAndDescription(root);

    root.checkUnknownKeys();
    return root.ok(res);
}

bool ButtonPositionSettingV3::load(const matjson::Value& json) {
    if (!json.contains("button-position")) {
        return false;
    }
    int positionNumber = json["button-position"].is_number() ? json["button-position"].as_int() : 0;
    // in case it's set to something stupid
    positionNumber = std::min(static_cast<int>(ButtonPosition::NUM_VALUES - 1), std::max(0, positionNumber));
    position = static_cast<ButtonPosition>(positionNumber);

    return true;
}

bool ButtonPositionSettingV3::save(matjson::Value& json) const {
    json = matjson::Object {
            {"button-position", static_cast<int>(position)}
    };
    return true;
}

SettingNodeV3* ButtonPositionSettingV3::createNode(float width) {
    return ButtonPositionSettingNodeV3::create(
        std::static_pointer_cast<ButtonPositionSettingV3>(shared_from_this()),
        width
    );
}

bool ButtonPositionSettingV3::isDefaultValue() const {
    return position == defaultPosition;
}

void ButtonPositionSettingV3::reset() {
    position = defaultPosition;
    markChanged();
}

ButtonPosition ButtonPositionSettingV3::getPosition() const {
    return position;
}

void ButtonPositionSettingV3::setPosition(ButtonPosition position) {
    this->position = position;
    markChanged();
}

$execute {
    (void)Mod::get()->registerCustomSettingType("button-position-v3", &ButtonPositionSettingV3::parse);
}




