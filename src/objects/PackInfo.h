#ifndef GDDLINTEGRATION_PACKINFO_H
#define GDDLINTEGRATION_PACKINFO_H

#include <string>
#include <vector>

class PackInfo {
    const std::vector<int> levels;
    const std::string packName;
    const std::string packIconPath;

public:
    PackInfo(const std::vector<int>& levels, const std::string& packName, const std::string& packIconPath);

    std::vector<int> getLevels() const;
    std::string getPackName() const;
    std::string getPackIconPath() const;
};


#endif //GDDLINTEGRATION_PACKINFO_H