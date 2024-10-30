#ifndef SKILLSETS_H
#define SKILLSETS_H

#include <vector>
#include <matjson.hpp>
#include <string>

class Skillsets {
    std::vector<std::pair<std::string, int>> skillsets{};

public:
    Skillsets() = default;

    explicit Skillsets(const matjson::Value& tagsJson);

    std::vector<std::string> getSkillsets() const;
};



#endif //SKILLSETS_H
