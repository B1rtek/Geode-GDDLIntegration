#ifndef GDDLINTEGRATION_SEARCHOBJECT_H
#define GDDLINTEGRATION_SEARCHOBJECT_H

#include <memory>
#include <string>

#include "searchsettings/TextSearchSetting.h"

class SearchObject {
    std::shared_ptr<TextSearchSetting> levelNameSetting = std::make_shared<TextSearchSetting>("search-name", 32);

public:
    SearchObject() = default;

    void readFromSaved();
    void saveToSaved();

    std::shared_ptr<TextSearchSetting> getLevelNameSetting();
};


#endif //GDDLINTEGRATION_SEARCHOBJECT_H