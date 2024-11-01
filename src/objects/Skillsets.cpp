#include "Skillsets.h"

#include <algorithm>
#include <iterator>

Skillsets::Skillsets(const matjson::Value &tagsJson) {
    if (!tagsJson.is_array()) {
        return;
    }
    const auto &tags = tagsJson.as_array();
    for (const auto &tag: tags) {
        if (!tag.contains("ReactCount") || !tag["ReactCount"].is_number() || !tag.contains("TagID") || !tag["TagID"].
                is_number()) {
            continue;
        }
        const int votes = tag["ReactCount"].as_int();
        const int tagID = tag["TagID"].as_int();
        skillsets.emplace_back(tagID, votes);
    }
    std::ranges::sort(skillsets,
                      [](const std::pair<int, int> &a, const std::pair<int, int> &b) { return a.second > b.second; });
}

std::vector<int> Skillsets::getSkillsets() const {
    std::vector<int> justSkillsets;
    int count = 0; // the site only shows top 3
    for (const auto &skillsetPair: skillsets) {
        justSkillsets.push_back(skillsetPair.first);
        ++count;
        if (count >= 3) {
            break;
        }
    }
    return justSkillsets;
}
