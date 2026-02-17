#ifndef GDDLINTEGRATION_BASESEARCHSETTING_H
#define GDDLINTEGRATION_BASESEARCHSETTING_H

#include <string>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

template<typename T>
class SearchSetting {
protected:
    T value;
    std::string settingKey;
public:
    explicit SearchSetting(const std::string& settingKey) {
        this->settingKey = settingKey;
        SearchSetting::getSavedSetting();
    }

	virtual ~SearchSetting() = default;

	virtual T getSettingValue() {
		return value;
	}

	virtual void setSettingValue(T value) {
		this->value = value;
	}

	virtual void getSavedSetting() {
		value = Mod::get()->getSavedValue<T>(settingKey, T{});
	}

	virtual void saveSetting() {
		Mod::get()->setSavedValue(settingKey, value);
	}
};

#endif //GDDLINTEGRATION_BASESEARCHSETTING_H