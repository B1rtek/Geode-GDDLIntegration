#ifndef GDDLINTEGRATION_DUMMYSETTINGCR_H
#define GDDLINTEGRATION_DUMMYSETTINGCR_H

#include <Geode/Prelude.hpp>
#include <Geode/loader/Setting.hpp>

using namespace geode::prelude;

class DummySettingCR : public SettingValue {
    DummySettingCR(std::string const &key, std::string const &modID) : SettingValue(key, modID) {}

    bool load(matjson::Value const& json) override;

    bool save(matjson::Value& json) const override;

    SettingNode* createNode(float width) override;
};


#endif //GDDLINTEGRATION_DUMMYSETTINGCR_H
