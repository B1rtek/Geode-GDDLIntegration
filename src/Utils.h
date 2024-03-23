#ifndef GDDL_UTILS_H
#define GDDL_UTILS_H
#include <chrono>
#include <string>
#include <sys/stat.h>

class Utils {
public:
    static std::string floatToString(float number, int precision) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << number;
        return stream.str();
    }

    static unsigned int getCurrentTimestamp() {
        const auto clockNow = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(clockNow.time_since_epoch()).count();
    }

    // I love stack overflow <3
    // https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
    static bool fileExists(const std::string &name) {
        struct stat buffer {};
        return stat(name.c_str(), &buffer) == 0;
    }

    // https://stackoverflow.com/questions/2390912/checking-for-an-empty-file-in-c
    static bool fileIsEmpty(std::ifstream& pFile)
    {
        return pFile.peek() == std::ifstream::traits_type::eof();
    }

    template<typename T>
    static std::vector<T> copySetToVector(std::set<T> setToCopy) {
        std::vector<T> vec;
        for (auto element: setToCopy) {
            vec.push_back(element);
        }
        return vec;
    }

    template<typename T>
    static std::set<T> copyVectorToSet(std::vector<T> vectorToCopy) {
        std::set<T> set;
        for (auto element: vectorToCopy) {
            set.insert(element);
        }
        return set;
    }
};
#endif // GDDL_UTILS_H
