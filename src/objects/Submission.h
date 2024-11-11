#ifndef GDDLINTEGRATION_SUBMISSION_H
#define GDDLINTEGRATION_SUBMISSION_H

#include <Geode/Bindings.hpp>

using namespace geode::prelude;


class Submission {
    int levelID, rating, enjoyment, refreshRate, device;
    std::string proof;
    int progress, attempts;
    bool isSolo;
    int secondPlayerID;
public:
    Submission() = default;

    Submission(matjson::Value json, bool request);

    std::string describe() const;
};


#endif //GDDLINTEGRATION_SUBMISSION_H
