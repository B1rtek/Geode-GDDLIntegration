#ifndef GDDLINTEGRATION_USEOLDTIERLABELSETTINGV3_H
#define GDDLINTEGRATION_USEOLDTIERLABELSETTINGV3_H

#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

class UseOldTierLabelSettingV3 final : public SettingV3 {
protected:
    bool enabled;
    int positionOffset;

public:
    static constexpr bool defaultEnabled = false;
    static constexpr int defaultPositionOffset = 0;
    static constexpr int minOffset = -1;
    static constexpr int maxOffset = 3;

    static Result<std::shared_ptr<UseOldTierLabelSettingV3>> parse(std::string const& key, std::string const& modID,
                                                                   matjson::Value const& json);

    bool load(const matjson::Value &json) override;
    bool save(matjson::Value &json) const override;
    SettingNodeV3 *createNode(float width) override;
    bool isDefaultValue() const override;
    void reset() override;

    bool isEnabled() const;
    void setEnabled(bool enabled);
    int getPositionOffset() const;
    void setPositionOffset(int positionOffset);
};

$execute {
    (void)Mod::get()->registerCustomSettingType("use-old-tier-label-v3", &UseOldTierLabelSettingV3::parse);
}


#endif //GDDLINTEGRATION_USEOLDTIERLABELSETTINGV3_H
