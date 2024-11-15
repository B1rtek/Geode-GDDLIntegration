#include "Skillsets.h"

#include <algorithm>
#include <iterator>

Skillsets::Skillsets(const matjson::Value &tagsJson) {
    if (!tagsJson.isArray()) {
        return;
    }
    const auto &tags = tagsJson.asArray().unwrap();
    for (const auto &tag: tags) {
        if (!tag.contains("ReactCount") || !tag["ReactCount"].isNumber() || !tag.contains("TagID") || !tag["TagID"].
                isNumber()) {
            continue;
        }
        const int votes = tag["ReactCount"].asInt().unwrap();
        const int tagID = tag["TagID"].asInt().unwrap();
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
