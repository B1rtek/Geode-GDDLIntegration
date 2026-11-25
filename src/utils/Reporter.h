#ifndef GDDLINTEGRATION_REPORTER_H
#define GDDLINTEGRATION_REPORTER_H

#include "LogMessage.h"
#include <source_location>

using namespace geode::prelude;

class Reporter {
public:
    Reporter() = delete;

    static void report(const LogMessage &message, std::source_location location = std::source_location::current());
};


#endif //GDDLINTEGRATION_REPORTER_H
