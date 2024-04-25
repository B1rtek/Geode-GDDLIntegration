#ifndef EXCLUDERANGESETTING_H
#define EXCLUDERANGESETTING_H

#include <Geode/loader/Setting.hpp>

using namespace geode::prelude;

class ExcludeRangeSetting : public SettingValue {
protected:
    int rangeBegin, rangeEnd;
    bool include;

public:
    ExcludeRangeSetting(std::string const &key, std::string const &modID, int rangeBegin, int rangeEnd,
                        bool include) : SettingValue(key, modID), rangeBegin(rangeBegin), rangeEnd(rangeBegin),
                                        include(include) {}

    bool load(matjson::Value const& json) override;

    bool save(matjson::Value& json) const override;

    SettingNode* createNode(float width) override;

    int getRangeBegin() const;

    void setRangeBegin(int rangeBegin);

    int getRangeEnd() const;

    void setRangeEnd(int rangeEnd);

    bool isInclude() const;

    void setInclude(bool include);
};


#endif //EXCLUDERANGESETTING_H
