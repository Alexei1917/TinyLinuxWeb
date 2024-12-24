#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <string>

class LogManager {
public:
    void logTraffic(const std::string &data);
    void logSystem(const std::string &data);
};

#endif