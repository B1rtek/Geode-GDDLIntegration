#ifndef GDDLINTEGRATION_INTRANGESEARCHSETTING_H
#define GDDLINTEGRATION_INTRANGESEARCHSETTING_H

#include "SearchSetting.h"
#include <vector>

template <typename T>
class RangeSearchSetting : public SearchSetting<std::vector<T>> {
protected:
    T minValue, maxValue;
    bool allowOverlap;

public:
    RangeSearchSetting(const std::string& settingKey, T minValue, T maxValue, const bool allowOverlap = true) :
        SearchSetting<std::vector<T>>(settingKey, std::vector<T>{minValue, maxValue}), minValue(minValue),
        maxValue(maxValue), allowOverlap(allowOverlap) {}

    void setSettingValue(std::vector<T> value) override {
        if (value.size() != 2) return;
        this->setValueNumber(0, value[0]);
        this->setValueNumber(1, value[1]);
    }

    void loadSetting() override {
        SearchSetting<std::vector<T>>::loadSetting();
        this->setSettingValue(this->value);
    }

    void setValueNumber(unsigned index, T value) {
        T newValue = value < minValue ? maxValue : value > maxValue ? minValue : value;
        if (!allowOverlap && newValue > this->value[1 - index]) newValue = this->value[1 - index];
        this->value[index] = newValue;
    }

    std::vector<T> getMinMaxValues() {
        return {minValue, maxValue};
    }
};


#endif //GDDLINTEGRATION_INTRANGESEARCHSETTING_H
