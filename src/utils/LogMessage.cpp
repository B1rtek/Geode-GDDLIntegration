#include "LogMessage.h"

std::string LogMessage::getLogContent() const {
    return message;
}

std::string LogMessage::getNotificationContent() const {
    return message.substr(0, 64);
}

bool LogMessage::showToast() const {
    return this->toast;
}

Severity LogMessage::getSeverity() const {
    return this->severity;
}
