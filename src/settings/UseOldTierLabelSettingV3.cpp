#include "UseOldTierLabelSettingV3.h"

#include "UseOldTierLabelSettingNodeV3.h"

Result<std::shared_ptr<UseOldTierLabelSettingV3>>
UseOldTierLabelSettingV3::parse(const std::string &key, const std::string &modID, const matjson::Value &json) {
    const auto res = std::make_shared<UseOldTierLabelSettingV3>();
    auto root = checkJson(json, "UseOldTierLabelSettingV3");

    res->init(key, modID, root);
    res->parseNameAndDescription(root);

    root.checkUnknownKeys();
    return root.ok(res);
}

bool UseOldTierLabelSettingV3::load(const matjson::Value &json) {
    if(!json.contains("enabled") || !json.contains("position-offset")) {
        return false;
    }
    enabled = json["enabled"].isBool() ? json["enabled"].asBool().unwrap() : false;
    positionOffset = json["position-offset"].isNumber() ? json["position-offset"].asInt().unwrap() : 0;
    // in case the value are set to something stupid
    positionOffset = std::min(maxOffset, std::max(minOffset, positionOffset));
    return true;
}

bool UseOldTierLabelSettingV3::save(matjson::Value &json) const {
    json = matjson::Object {
            {"enabled", enabled},
            {"position-offset", positionOffset}
    };
    return true;
}

SettingNodeV3 *UseOldTierLabelSettingV3::createNode(float width) {
    return UseOldTierLabelSettingNodeV3::create(
            std::static_pointer_cast<UseOldTierLabelSettingV3>(shared_from_this()),
            width
    );
}

bool UseOldTierLabelSettingV3::isDefaultValue() const {
    return enabled == defaultEnabled && positionOffset == defaultPositionOffset;
}

void UseOldTierLabelSettingV3::reset() {
    enabled = defaultEnabled;
    positionOffset = defaultPositionOffset;
    markChanged();
}

bool UseOldTierLabelSettingV3::isEnabled() const {
    return enabled;
}

void UseOldTierLabelSettingV3::setEnabled(bool enabled) {
    this->enabled = enabled;
    markChanged();
}

int UseOldTierLabelSettingV3::getPositionOffset() const {
    return positionOffset;
}

void UseOldTierLabelSettingV3::setPositionOffset(int positionOffset) {
    this->positionOffset = positionOffset;
    markChanged();
}

$execute {
    (void)Mod::get()->registerCustomSettingType("use-old-tier-label-v3", &UseOldTierLabelSettingV3::parse);
}
