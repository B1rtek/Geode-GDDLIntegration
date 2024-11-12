#include "ExcludeRangeSettingV3.h"
#include "ExcludeRangeSettingNodeV3.h"

Result<std::shared_ptr<ExcludeRangeSettingV3>>
ExcludeRangeSettingV3::parse(const std::string &key, const std::string &modID, const matjson::Value &json) {
    const auto res = std::make_shared<ExcludeRangeSettingV3>();
    auto root = checkJson(json, "ExcludeRangeSettingV3");

    res->init(key, modID, root);
    res->parseNameAndDescription(root);

    root.checkUnknownKeys();
    return root.ok(res);
}

bool ExcludeRangeSettingV3::load(const matjson::Value &json) {
    if (!json.contains("range-begin") || !json.contains("range-end") || !json.contains("include")) {
        return false;
    }
    rangeBegin = json["range-begin"].isNumber() ? json["range-begin"].asInt().unwrap() : 0;
    rangeEnd = json["range-end"].isNumber() ? json["range-end"].asInt().unwrap() : 0;
    include = json["include"].isBool() ? json["include"].asBool().unwrap() : false;
    // in case the values are set to something stupid
    rangeBegin = std::min(ExcludeRangeSettingV3::highestTier, std::max(0, rangeBegin));
    rangeEnd = std::min(ExcludeRangeSettingV3::highestTier, std::max(0, rangeEnd));
    return true;
}

bool ExcludeRangeSettingV3::save(matjson::Value &json) const {
    json = matjson::Object {
            {"range-begin", rangeBegin},
            {"range-end", rangeEnd},
            {"include", include}
    };
    return true;
}

SettingNodeV3 *ExcludeRangeSettingV3::createNode(float width) {
    return ExcludeRangeSettingNodeV3::create(
            std::static_pointer_cast<ExcludeRangeSettingV3>(shared_from_this()),
            width
    );
}

bool ExcludeRangeSettingV3::isDefaultValue() const {
    return rangeBegin == defaultRangeBegin && rangeEnd == defaultRangeEnd && include == defaultInclude;
}

void ExcludeRangeSettingV3::reset() {
    rangeBegin = defaultRangeBegin;
    rangeEnd = defaultRangeEnd;
    include = defaultInclude;
    markChanged();
}

int ExcludeRangeSettingV3::getRangeBegin() const {
    return rangeBegin;
}

void ExcludeRangeSettingV3::setRangeBegin(int rangeBegin) {
    this->rangeBegin = rangeBegin;
    markChanged();
}

int ExcludeRangeSettingV3::getRangeEnd() const {
    return rangeEnd;
}

void ExcludeRangeSettingV3::setRangeEnd(int rangeEnd) {
    this->rangeEnd = rangeEnd;
    markChanged();
}

bool ExcludeRangeSettingV3::isInclude() const {
    return include;
}

void ExcludeRangeSettingV3::setInclude(bool include) {
    this->include = include;
    markChanged();
}

$execute {
    (void)Mod::get()->registerCustomSettingType("exclude-range-v3", &ExcludeRangeSettingV3::parse);
}
