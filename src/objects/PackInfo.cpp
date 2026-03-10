#include "PackInfo.h"

#include "PackInfo.h"

#include <Geode/ui/Notification.hpp>

PackInfo::PackInfo(const int id, const int categoryId, const std::string& name, const std::string& iconPath,
                   const int medianTier): id(id), categoryId(categoryId), name(name), iconPath(iconPath), medianTier(medianTier) {
}

Result<PackInfo> PackInfo::createFromJson(const matjson::Value& json) {
    if (!json.isObject() ||
        !json.contains("ID") || !json["ID"].isNumber() ||
        !json.contains("CategoryID") || !json["CategoryID"].isNumber() ||
        !json.contains("Name") || !json["Name"].isString()) {
        return Err("Invalid PackInfo JSON");
    }
    std::string iconName = "tier_unrated.png";
    if (json.contains("IconName") && json["IconName"].isString()) {
        iconName = json["IconName"].asString().unwrap();
    }
    int medianTier = -1;
    if (json.contains("Meta") && json["Meta"].isObject() && json["Meta"].contains("MedianTier") && json["Meta"]["MedianTier"].isNumber()) {
        medianTier = json["Meta"]["MedianTier"].asInt().unwrap();
    }
    return Ok(PackInfo(json["ID"].asInt().unwrap(), json["CategoryID"].asInt().unwrap(), json["Name"].asString().unwrap(), iconName, medianTier));
}

int PackInfo::getId() const {
    return id;
}

int PackInfo::getCategoryId() const {
    return categoryId;
}

std::string PackInfo::getName() const {
    return name;
}

std::string PackInfo::getIconPath() const {
    return iconPath;
}

int PackInfo::getMedianTier() const {
    return medianTier;
}

std::vector<int> PackInfo::getLevels() const {
    return levels;
}
