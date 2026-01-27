#ifndef GDDLINTEGRATION_SUBMISSION_H
#define GDDLINTEGRATION_SUBMISSION_H

#include <matjson.hpp>
#include <Geode/Bindings.hpp>

using namespace geode::prelude;


class Submission {
    int levelID, rating, enjoyment, refreshRate, device;
    std::string proof;
    int progress = -1, attempts;
    bool isSolo;
    int secondPlayerID;
public:
    Submission() = default;

    Submission(matjson::Value json, bool request);

    bool isEmpty() const;

    std::string describe() const;
};


#endif //GDDLINTEGRATION_SUBMISSION_H
