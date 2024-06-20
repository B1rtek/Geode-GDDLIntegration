#include "ExcludeRangeSetting.h"

#include "ExcludeRangeSettingNode.h"

bool ExcludeRangeSetting::load(matjson::Value const &json) {
    if (!json.contains("range-begin") || !json.contains("range-end") || !json.contains("include")) {
        return false;
    }
    rangeBegin = json["range-begin"].is_number() ? json["range-begin"].as_int() : 0;
    rangeEnd = json["range-end"].is_number() ? json["range-end"].as_int() : 0;
    include = json["include"].is_bool() ? json["include"].as_bool() : false;
    // in case the values are set to something stupid
    rangeBegin = std::min(ExcludeRangeSettingNode::highestTier, std::max(0, rangeBegin));
    rangeEnd = std::min(ExcludeRangeSettingNode::highestTier, std::max(0, rangeEnd));
    return true;
}

bool ExcludeRangeSetting::save(matjson::Value &json) const {
    json = matjson::Object {
        {"range-begin", rangeBegin},
        {"range-end", rangeEnd},
        {"include", include}
    };
    return true;
}

SettingNode * ExcludeRangeSetting::createNode(float width) {
    return ExcludeRangeSettingNode::create(this, width);
}

int ExcludeRangeSetting::getRangeBegin() const {
    return rangeBegin;
}

void ExcludeRangeSetting::setRangeBegin(int rangeBegin) {
    this->rangeBegin = rangeBegin;
}

int ExcludeRangeSetting::getRangeEnd() const {
    return rangeEnd;
}

void ExcludeRangeSetting::setRangeEnd(int rangeEnd) {
    this->rangeEnd = rangeEnd;
}

bool ExcludeRangeSetting::isInclude() const {
    return include;
}

void ExcludeRangeSetting::setInclude(bool include) {
    this->include = include;
}
