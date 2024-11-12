#include "DummySettingLoginV3.h"
#include "LoginSettingNodeV3.h"

Result<std::shared_ptr<SettingV3>>
DummySettingLoginV3::parse(const std::string& key, const std::string& modID, const matjson::Value& json) {
    const auto res = std::make_shared<DummySettingLoginV3>();
    auto root = checkJson(json, "DummySettingLoginV3");

    res->init(key, modID, root);
    res->parseNameAndDescription(root);

    root.checkUnknownKeys();
    return root.ok(std::static_pointer_cast<SettingV3>(res));
}

bool DummySettingLoginV3::load(const matjson::Value &json) {
    return true;
}

bool DummySettingLoginV3::save(matjson::Value &json) const {
    return true;
}

bool DummySettingLoginV3::isDefaultValue() const {
    return true;
}

void DummySettingLoginV3::reset() {

}

SettingNodeV3 *DummySettingLoginV3::createNode(float width) {
    return LoginSettingNodeV3::create(
            std::static_pointer_cast<DummySettingLoginV3>(shared_from_this()),
            width
    );
}

$execute {
    (void)Mod::get()->registerCustomSettingType("login-v3", &DummySettingLoginV3::parse);
}
