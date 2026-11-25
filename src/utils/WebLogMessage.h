#ifndef GDDLINTEGRATION_WEBLOGMESSAGE_H
#define GDDLINTEGRATION_WEBLOGMESSAGE_H

#include "LogMessage.h"

#include <utility>

class WebLogMessage : public LogMessage {
    int httpCode;
    web::WebResponse res;
public:
    WebLogMessage(const Severity severity, const std::string& message, const int httpCode, web::WebResponse  res, const bool toast = true): LogMessage(severity, message, toast), httpCode(httpCode), res(std::move(res)) {}

    std::string getLogContent() const override;
    std::string getNotificationContent() const override;
};


#endif //GDDLINTEGRATION_WEBLOGMESSAGE_H
