#ifndef USEOLDTIERLABELSETTING_H
#define USEOLDTIERLABELSETTING_H

#include <Geode/Prelude.hpp>
#include <Geode/loader/Setting.hpp>

using namespace geode::prelude;

class UseOldTierLabelSetting : public SettingValue {
protected:
    bool enabled;
    int positionOffset;

public:
    UseOldTierLabelSetting(std::string const &key, std::string const &modID, bool enabled, int positionOffset) : SettingValue(key, modID), enabled(enabled), positionOffset(positionOffset) {}

    bool load(matjson::Value const& json) override;

    bool save(matjson::Value& json) const override;

    SettingNode* createNode(float width) override;

    bool isEnabled() const;

    void setEnabled(bool enabled);

    int getPositionOffset() const;

    void setPositionOffset(int positionOffset);
};



#endif //USEOLDTIERLABELSETTING_H
