#ifndef GDDLINTEGRATION_INTRANGESEARCHSETTING_H
#define GDDLINTEGRATION_INTRANGESEARCHSETTING_H

#include "SearchSetting.h"
#include <vector>

template <typename T>
class RangeSearchSetting : public SearchSetting<std::vector<T>> {
protected:
    T minValue, maxValue;
    bool allowOverlap, wrapAround;
    std::string searchQueryParameterMin, searchQueryParameterMax;
public:
    RangeSearchSetting(const std::string& settingKey, T minValue, T maxValue, const std::string& searchQueryParameterNameMin,
        const std::string& searchQueryParameterNameMax, std::vector<T> defaultValue = {}, const bool allowOverlap = true,
        const bool wrapAround = true) : SearchSetting<std::vector<T>>(settingKey, std::vector<T>{minValue, maxValue}),
        minValue(minValue), maxValue(maxValue), searchQueryParameterMin(searchQueryParameterNameMin),
        searchQueryParameterMax(searchQueryParameterNameMax), allowOverlap(allowOverlap), wrapAround(wrapAround) {
        if (defaultValue.size() > 0) {
            this->defaultValue = defaultValue;
        }
    }

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
        T newValue;
        if (wrapAround) {
            newValue = value < minValue ? maxValue : value > maxValue ? minValue : value;
        } else {
            newValue = value < minValue ? minValue : value > maxValue ? maxValue : value;
        }
        if (!allowOverlap && newValue > this->value[1 - index]) newValue = this->value[1 - index];
        this->value[index] = newValue;
    }

    std::vector<T> getMinMaxValues() {
        return {minValue, maxValue};
    }

    std::string getSearchQueryFragment() override {
        std::string fragment = "";
        if (this->value[0] != this->defaultValue[0]) {
            fragment += "&" + this->searchQueryParameterMin + "=" + std::to_string(this->value[0]);
        }
        if (this->value[1] != this->defaultValue[1]) {
            fragment += "&" + this->searchQueryParameterMax + "=" + std::to_string(this->value[1]);
        }
        return fragment;
    }
};


#endif //GDDLINTEGRATION_INTRANGESEARCHSETTING_H
