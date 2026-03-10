#include "PackCategoryInfo.h"

PackCategoryInfo::PackCategoryInfo(const int id, const std::string& name, const std::string& description) : id(id), name(name), description(description) {}

geode::Result<PackCategoryInfo> PackCategoryInfo::createFromJson(const matjson::Value& json) {
    if (!json.isObject() ||
        !json.contains("ID") || !json["ID"].isNumber() ||
        !json.contains("Name") || !json["Name"].isString() ||
        !json.contains("Description") || !json["Description"].isString()) {
        return geode::Err("Invalid PackCategoryInfo JSON");
    }
    return geode::Ok(PackCategoryInfo(json["ID"].asInt().unwrap(), json["Name"].asString().unwrap(), json["Description"].asString().unwrap()));
}

int PackCategoryInfo::getId() const {
    return id;
}

const std::string& PackCategoryInfo::getName() const {
    return name;
}

const std::string& PackCategoryInfo::getDescription() const {
    return description;
}

