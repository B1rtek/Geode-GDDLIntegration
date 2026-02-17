#ifndef GDDLINTEGRATION_BASESEARCHSETTING_H
#define GDDLINTEGRATION_BASESEARCHSETTING_H

#include <string>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

template<typename T>
class SearchSetting {
protected:
    T value;
	T defaultValue;
    std::string settingKey;
public:
    explicit SearchSetting(const std::string& settingKey, T defaultValue = T{}) {
        this->settingKey = settingKey;
    	this->defaultValue = defaultValue;
    }

	virtual ~SearchSetting() = default;

	virtual T getSettingValue() {
		return value;
	}

	virtual void setSettingValue(T value) {
		this->value = value;
	}

	virtual void loadSetting() {
		value = Mod::get()->getSavedValue<T>(settingKey, defaultValue);
	}

	virtual void saveSetting() {
		Mod::get()->setSavedValue(settingKey, value);
	}
};

#endif //GDDLINTEGRATION_BASESEARCHSETTING_H