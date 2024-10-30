#include "Skillsets.h"

#include <algorithm>
#include <iterator>

Skillsets::Skillsets(const matjson::Value& tagsJson) {
    if (!tagsJson.is_array()) {
        return;
    }
    const auto& tags = tagsJson.as_array();
    for (const auto& tag : tags) {
        if (!tag.contains("ReactCount") || !tag["ReactCount"].is_number() || !tag.contains("Tag") || !tag["Tag"].
            is_object()) {
            continue;
        }
        const auto& tagDetails = tag["Tag"];
        if (!tagDetails.contains("Name") || !tagDetails["Name"].is_string()) {
            continue;
        }
        const int votes = tag["ReactCount"].as_int();
        const std::string name = tagDetails["Name"].as_string();
        skillsets.emplace_back(name, votes);
    }
    std::ranges::sort(skillsets,
                      [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                          return a.second < b.second;
                      });
}

std::vector<std::string> Skillsets::getSkillsets() const {
    std::vector<std::string> justSkillsets;
    std::ranges::transform(skillsets, justSkillsets.begin(), [](const std::pair<std::string, int>& element) {
        return element.first;
    });
    return justSkillsets;
}
