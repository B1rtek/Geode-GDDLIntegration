#ifndef GDDLINTEGRATION_PACKINFO_H
#define GDDLINTEGRATION_PACKINFO_H

#include <matjson.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class PackInfo {
    int id{};
    int categoryId{};
    std::string name;
    std::string iconPath;
    int medianTier{};
    std::vector<int> levels;
    
public:
    PackInfo() = default;

    PackInfo(const int id, const int categoryId, const std::string& name, const std::string& iconPath, const int medianTier);

    static Result<PackInfo> createFromJson(const matjson::Value& json);

    int getId() const;
    int getCategoryId() const;
    std::string getName() const;
    std::string getIconPath() const;
    int getMedianTier() const;
    std::vector<int> getLevels() const;
};


#endif //GDDLINTEGRATION_PACKINFO_H