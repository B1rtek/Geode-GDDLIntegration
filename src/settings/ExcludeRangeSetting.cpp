#include "ExcludeRangeSetting.h"

bool ExcludeRangeSetting::load(matjson::Value const &json) {
    if (!json.contains("range-begin") || !json.contains("range-end") || !json.contains("include")) {
        return false;
    }
    rangeBegin = json["range-begin"].as_int();
    rangeEnd = json["range-end"].as_int();
    include = json["include"].as_bool();
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
