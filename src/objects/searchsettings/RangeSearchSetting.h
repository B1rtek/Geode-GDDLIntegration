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
        this->setMinValue(value[0]);
        this->setMaxValue(value[1]);
    }

    void loadSetting() override {
        SearchSetting<T>::loadSetting();
        this->setSettingValue(this->value);
    }

    void setMinValue(T value) {
        const T newValue = value < minValue ? maxValue : value;
        if (!allowOverlap && newValue > this->value[1]) return;
        this->value[0] = newValue;
    }

    void setMaxValue(T value) {
        const T newValue = value > maxValue ? minValue : value;
        if (!allowOverlap && newValue < this->value[0]) return;
        this->value[1] = newValue;
    }

    std::pair<bool, bool> isAtMinValue() {
        return std::make_pair(this->value[0] == minValue, this->value[1] == minValue);
    }
};


#endif //GDDLINTEGRATION_INTRANGESEARCHSETTING_H
