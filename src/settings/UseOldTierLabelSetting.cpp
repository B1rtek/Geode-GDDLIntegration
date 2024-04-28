#include "UseOldTierLabelSetting.h"


bool UseOldTierLabelSetting::load(matjson::Value const &json) {
    if(!json.contains("enabled") || !json.contains("position-offset")) {
        return false;
    }
    enabled = json["enabled"].as_bool();
    positionOffset = json["position-offset"].as_int();
    return true;
}

bool UseOldTierLabelSetting::save(matjson::Value &json) const {
    json = matjson::Object {
        {"enabled", enabled},
        {"position-offset", positionOffset}
    };
    return true;
}

SettingNode * UseOldTierLabelSetting::createNode(float width) {
    // return UseOldTierLabelSettingNode::create(this, width);
}

bool UseOldTierLabelSetting::isEnabled() const {
    return enabled;
}

void UseOldTierLabelSetting::setEnabled(bool enabled) {
    this->enabled = enabled;
}

int UseOldTierLabelSetting::getPositionOffset() const {
    return positionOffset;
}

void UseOldTierLabelSetting::setPositionOffset(int positionOffset) {
    this->positionOffset = positionOffset;
}
