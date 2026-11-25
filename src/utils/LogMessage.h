#ifndef GDDLINTEGRATION_LOGMESSAGE_H
#define GDDLINTEGRATION_LOGMESSAGE_H

#include <utility>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class LogMessage {
protected:
    Severity severity;
    std::string message;
    bool toast;
public:
    LogMessage(const Severity severity, std::string message, const bool toast = true) : severity(severity), message(std::move(message)), toast(toast) {}
    virtual ~LogMessage() = default;

    virtual std::string getLogContent() const;
    virtual std::string getNotificationContent() const;

    bool showToast() const;
    Severity getSeverity() const;
};


#endif //GDDLINTEGRATION_LOGMESSAGE_H
