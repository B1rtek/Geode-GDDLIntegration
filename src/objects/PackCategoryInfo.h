#ifndef GDDLINTEGRATION_PACKCATEGORYINFO_H
#define GDDLINTEGRATION_PACKCATEGORYINFO_H

#include <matjson.hpp>
#include <string>

class PackCategoryInfo {
    int id{};
    std::string name;
    std::string description;

public:
    PackCategoryInfo() = default;

    PackCategoryInfo(int id, const std::string& name, const std::string& description);

    static geode::Result<PackCategoryInfo> createFromJson(const matjson::Value& json);

    int getId() const;
    const std::string& getName() const;
    const std::string& getDescription() const;
};


#endif //GDDLINTEGRATION_PACKCATEGORYINFO_H