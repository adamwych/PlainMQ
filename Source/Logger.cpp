#include "Logger.hpp"
#include "Core/EnvironmentSettings.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include <filesystem>
#include <chrono>

using namespace PlainMQ;

static bool _loggerInitialized;
static std::string _logFilePath;
static EnvironmentSettings _settings;

#define LOGGER_BUFFER_SIZE 4096

// Forward declarations.
void LogMessage(FILE* output, const std::string& message);
std::string FormatMessage(const std::string& prefix, std::string message, va_list list);
std::string GetFormattedDate();
std::string GetFormattedTime();
inline std::string AddLeadingZero(int num);

void Logger::Initialize(const EnvironmentSettings& settings) {
    if (_loggerInitialized) {
        Logger::LogWarning("Logger has already been initialized. Ignoring.");
        return;
    }
    
    _loggerInitialized = true;
    _settings = settings;
    _logFilePath = settings.logsDirectory + "/" + GetFormattedDate() + ".log";

    // Create parent directories if they don't exist, yet.
    std::filesystem::create_directories(settings.logsDirectory);
}

void Logger::LogInfo(std::string format, ...) {
    va_list list;
    va_start(list, format);
    LogMessage(stdout, FormatMessage("INFO", format, list));
    va_end(list);
}

void Logger::LogError(std::string format, ...) {
    va_list list;
    va_start(list, format);
    LogMessage(stderr, FormatMessage("ERROR", format, list));
    va_end(list);
}

void Logger::LogWarning(std::string format, ...) {
    va_list list;
    va_start(list, format);
    LogMessage(stdout, FormatMessage("WARNING", format, list));
    va_end(list);
}

// Internal function that automatically logs given message to
// the output and the log file, if logging to file is enabled in environment settings.
void LogMessage(FILE* output, const std::string& message) {
    fprintf(output, "%s", message.c_str());

    if (_loggerInitialized && _settings.logToFileEnabled) {
        auto outputFileHandle = fopen(_logFilePath.c_str(), "a+");
        if (outputFileHandle) {
            fprintf(outputFileHandle, "%s", message.c_str());
            fclose(outputFileHandle);
        } else {
            fprintf(stderr, "[ERROR] Unable to write logs to file %s\n", _logFilePath.c_str());
        }
    }
}

// Formats specified message using varargs.
std::string FormatMessage(const std::string& prefix, std::string message, va_list list) {
    char buffer[LOGGER_BUFFER_SIZE];
    std::string format = "[" + prefix + "][" + GetFormattedDate() + " " + GetFormattedTime() + "] " + message + "\n";

    vsprintf(buffer, format.c_str(), list);

    return std::string(buffer);
}

std::string GetFormattedDate() {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto date = gmtime(&now);
    return AddLeadingZero(date->tm_mday) + "." + AddLeadingZero(date->tm_mon + 1) + "." + AddLeadingZero(1900 + date->tm_year);
}

std::string GetFormattedTime() {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto date = gmtime(&now);
    return AddLeadingZero(date->tm_hour) + ":" + AddLeadingZero(date->tm_min) + ":" + AddLeadingZero(date->tm_sec);
}

inline std::string AddLeadingZero(int num) {
    if (num < 10) {
        return "0" + std::to_string(num);
    }

    return std::to_string(num);
}