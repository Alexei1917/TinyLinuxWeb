#include "log_manager.h"
#include <fstream>
#include <iostream>

void LogManager::logTraffic(const std::string &data) {
    std::ofstream logFile("traffic.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << data << std::endl;
    }
}

void LogManager::logSystem(const std::string &data) {
    std::ofstream logFile("system.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << data << std::endl;
    }
}
