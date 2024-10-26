#include "DummySettingV3.h"
#include "CacheResetSettingNodeV3.h"

Result<std::shared_ptr<DummySettingV3>>
DummySettingV3::parse(const std::string &key, const std::string &modID, const matjson::Value &json) {
    const auto res = std::make_shared<DummySettingV3>();
    auto root = checkJson(json, "DummySettingV3");

    res->init(key, modID, root);
    res->parseNameAndDescription(root);

    root.checkUnknownKeys();
    return root.ok(res);
}

bool DummySettingV3::load(const matjson::Value &json) {
    return true;
}

bool DummySettingV3::save(matjson::Value &json) const {
    return true;
}

bool DummySettingV3::isDefaultValue() const {
    return true;
}

void DummySettingV3::reset() {

}

SettingNodeV3 *DummySettingV3::createNode(float width) {
    return CacheResetSettingNodeV3::create(
            std::static_pointer_cast<DummySettingV3>(shared_from_this()),
            width
    );
}

$execute {
    (void)Mod::get()->registerCustomSettingType("cache-reset-v3", &DummySettingV3::parse);
}
