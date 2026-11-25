#include "WebLogMessage.h"

std::string WebLogMessage::getLogContent() const {
    return "WebLogMessage" + LogMessage::getLogContent();
}

std::string WebLogMessage::getNotificationContent() const {
    return "WebLogMessage" + LogMessage::getNotificationContent();
}
