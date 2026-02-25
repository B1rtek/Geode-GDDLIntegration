#ifndef GDDLINTEGRATION_INTRANGEINPUTCONTROL_H
#define GDDLINTEGRATION_INTRANGEINPUTCONTROL_H

#include <objects/searchsettings/RangeSearchSetting.h>
#include "SearchInputControl.h"

template<typename T>
class RangeInputControl : public SearchInputControl {
    TextInput* minTextInput = nullptr;
    TextInput* maxTextInput = nullptr;
    std::shared_ptr<RangeSearchSetting<T>> relatedSetting;
    bool placeholderForMinValue = false;
    std::string placeholderContent;

    bool init(const std::string& labelText, const std::shared_ptr<RangeSearchSetting<T>>& relatedSetting,
              bool placeholderForMinValue = false, std::string placeholderContent = "") {
        if (!SearchInputControl::init()) return false;

        // association with setting and calculation of parameters
        this->relatedSetting = relatedSetting;
        const std::vector<T> minMaxValues = relatedSetting->getMinMaxValues();
        const std::vector<std::string> minMaxValuesRepresentations = {formatTextfieldContent(minMaxValues[0]), formatTextfieldContent(minMaxValues[1])};
        const unsigned maxCharacters = std::max(minMaxValuesRepresentations[0].size(), minMaxValuesRepresentations[1].size());
        this->placeholderForMinValue = placeholderForMinValue;
        this->placeholderContent = placeholderContent;
        // the actual UI of the control
        Utils::createLabel(controlMenu, "bigFont.fnt", labelText, standardComponentWidth, {controlSize.x/2, 45.0f});
        Utils::createGeodeTextInput(controlMenu, minTextInput, "bigFont.fnt", "", maxCharacters, {35.0f, standardComponentHeight}, {controlSize.x/2 - 37.5, 15.0f});
        Utils::createGeodeTextInput(controlMenu, maxTextInput, "bigFont.fnt", "", maxCharacters, {35.0f, standardComponentHeight}, {controlSize.x/2 + 37.5, 15.0f});
        minTextInput->setFilter("0123456789" + minMaxValuesRepresentations[0] + minMaxValuesRepresentations[1] + placeholderContent);
        maxTextInput->setFilter("0123456789" + minMaxValuesRepresentations[0] + minMaxValuesRepresentations[1] + placeholderContent);
        Utils::createLabel(controlMenu, "chatFont.fnt", "to", 30.0f, {controlSize.x/2, 15.0f});
        Utils::createLeftRightButtonsAround(minTextInput, {13.0f, 19.0f}, this, menu_selector(RangeInputControl::onMinValueLeft), menu_selector(RangeInputControl::onMinValueRight));
        Utils::createLeftRightButtonsAround(maxTextInput, {13.0f, 19.0f}, this, menu_selector(RangeInputControl::onMaxValueLeft), menu_selector(RangeInputControl::onMaxValueRight));
        // display current parameters
        this->loadSetting();

        return true;
    }

    std::vector<T> getCurrentValues() {
        const T defaultValue = relatedSetting->getMinMaxValues()[0];
        const T minValue = placeholderForMinValue && minTextInput->getString() == placeholderContent ? defaultValue : numFromString<T>(minTextInput->getString()).unwrapOr(defaultValue);
        const T maxValue = placeholderForMinValue && maxTextInput->getString() == placeholderContent ? defaultValue : numFromString<T>(maxTextInput->getString()).unwrapOr(defaultValue);
        return {minValue, maxValue};
    }

    T subtract(T value) {
        T one = T{};
        ++one;
        const std::vector<T> minMaxValues = relatedSetting->getMinMaxValues();
        if (value > minMaxValues[0] && value - one < minMaxValues[0]) {
            // if less than 1 step from limit, set to limit
            return minMaxValues[0];
        }
        // otherwise just subtract
        return value - one;
    }

    T add(T value) {
        T one = T{};
        ++one;
        const std::vector<T> minMaxValues = relatedSetting->getMinMaxValues();
        if (value < minMaxValues[1] && value + one > minMaxValues[1]) {
            // if less than 1 step from limit, set to limit
            return minMaxValues[1];
        }
        // otherwise just add
        return value + one;
    }

    std::string formatTextfieldContent(T value) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << value;
        return ss.str();
    }

    void setTextfieldContent(TextInput* textfield, T value) {
        const T defaultValue = relatedSetting->getMinMaxValues()[0];
        if (placeholderForMinValue && value == defaultValue) {
            textfield->setString(placeholderContent);
        }        else {
            textfield->setString(formatTextfieldContent(value));
        }
    }

    void setSettingAndUpdateFields(std::vector<T> newValue) {
        relatedSetting->setSettingValue(newValue);
        const std::vector<T> newValues = relatedSetting->getSettingValue();
        setTextfieldContent(minTextInput, newValues[0]);
        setTextfieldContent(maxTextInput, newValues[1]);
    }

    void onMinValueLeft(CCObject* sender) {
        std::vector<T> currentValues = getCurrentValues();
        currentValues[0] = subtract(currentValues[0]);
        setSettingAndUpdateFields(currentValues);
    }

    void onMinValueRight(CCObject* sender) {
        std::vector<T> currentValues = getCurrentValues();
        currentValues[0] = add(currentValues[0]);
        setSettingAndUpdateFields(currentValues);
    }

    void onMaxValueLeft(CCObject* sender) {
        std::vector<T> currentValues = getCurrentValues();
        currentValues[1] = subtract(currentValues[1]);
        setSettingAndUpdateFields(currentValues);
    }

    void onMaxValueRight(CCObject* sender) {
        std::vector<T> currentValues = getCurrentValues();
        currentValues[1] = add(currentValues[1]);
        setSettingAndUpdateFields(currentValues);
    }

public:
    static RangeInputControl* create(const std::string& labelText,
                                     const std::shared_ptr<RangeSearchSetting<T>>& relatedSetting,
                                     bool placeholderForMinValue = false, std::string placeholderContent = "") {
        if (const auto newNode = new RangeInputControl<T>(); newNode != nullptr && newNode->init(
            labelText, relatedSetting, placeholderForMinValue, placeholderContent)) {
            newNode->autorelease();
            return newNode;
        }
        else {
            delete newNode;
            return nullptr;
        }
    }

    void saveSetting() override {
        this->relatedSetting->setSettingValue(getCurrentValues());
    }

    void loadSetting() override {
        this->setSettingAndUpdateFields(this->relatedSetting->getSettingValue());
    }
};


#endif //GDDLINTEGRATION_INTRANGEINPUTCONTROL_H
