#include <string>

class Utils {
public:
    static std::string floatToString(float number, int precision) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << number;
        return stream.str();
    }
};