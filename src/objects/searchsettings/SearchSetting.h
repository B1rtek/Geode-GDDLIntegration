#ifndef GDDLINTEGRATION_BASESEARCHSETTING_H
#define GDDLINTEGRATION_BASESEARCHSETTING_H

#include <string>
#include <Geode/loader/Mod.hpp>
#include "ISetting.h"

using namespace geode::prelude;

template<typename T>
class SearchSetting : public ISetting {
protected:
    T value;
	T defaultValue;
    std::string settingKey, searchQueryParameterName;
public:
    explicit SearchSetting(const std::string& settingKeyBase, T defaultValue = T{}) {
        this->settingKey = "search-" + settingKeyBase;
    	this->searchQueryParameterName = settingKeyBase;
    	this->defaultValue = defaultValue;
    	this->value = defaultValue;
    }

	~SearchSetting() override = default;

	virtual T getSettingValue() {
		return value;
	}

	virtual void setSettingValue(T value) {
		this->value = value;
	}

	void loadSetting() override {
		value = Mod::get()->getSavedValue<T>(settingKey, defaultValue);
	}

	void saveSetting() override {
		Mod::get()->setSavedValue(settingKey, value);
	}

    void resetToDefault() override {
		this->value = defaultValue;
	}

    std::string getSearchQueryFragment() override {
		return "";
	}
};

#endif //GDDLINTEGRATION_BASESEARCHSETTING_H