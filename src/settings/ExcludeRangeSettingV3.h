#ifndef EXCLUDERANGESETTINGV3_H
#define EXCLUDERANGESETTINGV3_H

#include <Geode/loader/SettingV3.hpp>
#include "Utils.h"

using namespace geode::prelude;

class ExcludeRangeSettingV3 final : public SettingV3 {
protected:
    int rangeBegin{}, rangeEnd{};
    bool include{};

public:
    static constexpr int defaultRangeBegin = 0;
    static constexpr int defaultRangeEnd = 0;
    static constexpr bool defaultInclude = false;
    static constexpr int highestTier = 35;

    static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID,
                                                    matjson::Value const& json);
    bool load(const matjson::Value& json) override;
    bool save(matjson::Value& json) const override;
    SettingNodeV3* createNode(float width) override;
    bool isDefaultValue() const override;
    void reset() override;

    int getRangeBegin() const;
    void setRangeBegin(int rangeBegin);
    int getRangeEnd() const;
    void setRangeEnd(int rangeEnd);
    bool isInclude() const;
    void setInclude(bool include);
};


#endif //EXCLUDERANGESETTINGV3_H
