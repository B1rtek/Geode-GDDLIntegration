#ifndef GDDLINTEGRATION_TEXTINPUTCONTROL_H
#define GDDLINTEGRATION_TEXTINPUTCONTROL_H

#include "SearchInputControl.h"


class TextInputControl : public SearchInputControl {
    TextInput* textInputNode = nullptr;

    bool init(const std::string& labelText);
public:
    static TextInputControl *create(const std::string& labelText);
};


#endif //GDDLINTEGRATION_TEXTINPUTCONTROL_H