#ifndef GDDLINTEGRATION_TEXTINPUTCONTROL_H
#define GDDLINTEGRATION_TEXTINPUTCONTROL_H

#include <objects/searchsettings/SearchSetting.h>

#include "SearchInputControl.h"


class TextInputControl : public SearchInputControl {
    TextInput* textInputNode = nullptr;
    std::shared_ptr<SearchSetting<std::string>> relatedSetting;

    bool init(const std::string& labelText, const std::shared_ptr<SearchSetting<std::string>>& relatedSetting);
public:
    static TextInputControl *create(const std::string& labelText, const std::shared_ptr<SearchSetting<std::string>>& relatedSetting);

    void saveSetting() override;
    void loadSetting() override;
};


#endif //GDDLINTEGRATION_TEXTINPUTCONTROL_H