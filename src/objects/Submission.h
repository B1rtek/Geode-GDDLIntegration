#ifndef GDDLINTEGRATION_SUBMISSION_H
#define GDDLINTEGRATION_SUBMISSION_H

#include <matjson.hpp>
#include <Geode/Bindings.hpp>

using namespace geode::prelude;


class Submission {
    int levelID, rating, enjoyment, refreshRate, device, status;
    std::string proof;
    int progress = -1, attempts;
    bool isSolo;
    int secondPlayerID;
public:
    const inline static std::vector<std::string> statusDisplay = {"Completed", "In progress", "On hold", "Dropped", "Plan to beat"};
    const inline static std::vector<std::string> statusValue = {"beaten", "beating", "hold", "dropped", "ptb"};

    Submission() = default;

    Submission(matjson::Value json, bool request);

    bool isEmpty() const;

    std::string describe() const;
};


#endif //GDDLINTEGRATION_SUBMISSION_H
