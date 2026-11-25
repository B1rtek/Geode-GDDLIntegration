#include "Reporter.h"

void Reporter::report(const LogMessage& message, std::source_location location) {
    const std::string locationStr = fmt::format("{} > {}:{}", location.file_name(), location.function_name(), location.line());
    auto icon = NotificationIcon::None;
    switch (message.getSeverity()) {
        case Severity::Info:
            log::info("{}: {}", locationStr, message.getLogContent());
            icon = NotificationIcon::Info;
        break;
        case Severity::Warning:
            log::warn("{}: {}", locationStr, message.getLogContent());
            icon = NotificationIcon::Warning;
        break;
        case Severity::Error:
            log::error("{}: {}", locationStr, message.getLogContent());
            icon = NotificationIcon::Error;
        break;
        default:
            log::error("[REACHED DEFAULT] {}: {}", locationStr, message.getLogContent());
        break;
    }
    if (message.showToast()) {
        Notification::create(message.getNotificationContent(), icon, message.getSeverity() == Severity::Error ? 3 : 2)->show();
    }
}
