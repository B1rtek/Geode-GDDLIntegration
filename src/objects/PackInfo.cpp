#include "PackInfo.h"

PackInfo::PackInfo(const std::vector<int>& levels, const std::string& packName, const std::string& packIconPath) : levels(levels), packName(packName), packIconPath(packIconPath) {

}

std::vector<int> PackInfo::getLevels() const {
    return levels;
}

std::string PackInfo::getPackName() const {
    return packName;
}

std::string PackInfo::getPackIconPath() const {
    return packIconPath;
}
