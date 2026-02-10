#ifndef GDDLINTEGRATION_SEARCHOBJECT_H
#define GDDLINTEGRATION_SEARCHOBJECT_H

#include <string>

class SearchObject {
    std::string levelName = "";

public:
    SearchObject() = default;

    void readFromSaved();
    void saveToSaved();

    std::string setLevelName(std::string input);
    std::string getLevelName();
};


#endif //GDDLINTEGRATION_SEARCHOBJECT_H