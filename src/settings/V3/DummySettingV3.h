#ifndef GDDLINTEGRATION_DUMMYSETTINGV3_H
#define GDDLINTEGRATION_DUMMYSETTINGV3_H

#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

class DummySettingV3 final : public SettingV3 {
public:
    static Result<std::shared_ptr<DummySettingV3>>
    parse(std::string const &key, std::string const &modID, matjson::Value const &json);

    bool load(matjson::Value const &json) override;
    bool save(matjson::Value &json) const override;
    bool isDefaultValue() const override;
    void reset() override;
    SettingNodeV3 *createNode(float width) override;
};


#endif //GDDLINTEGRATION_DUMMYSETTINGV3_H
