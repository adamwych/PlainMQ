#pragma once

#include <stdio.h>
#include <string>
#include <stdarg.h>

namespace PlainMQ {
    struct EnvironmentSettings;
    class Logger {
        public:

            /**
             * Initializes logger using given environmenet settings.
             * This should be the very first thing called when starting the application.
             */
            static void Initialize(const EnvironmentSettings& settings);

            /**
             * Logs specified message to stdout and log file, if logging
             * to file is enabled in environment settings.
             * Message will be prefixed with INFO and current time.
             */
            static void LogInfo(std::string format, ...);

            /**
             * Logs specified message to stderr and log file, if logging
             * to file is enabled in environment settings.
             * Message will be prefixed with ERROR and current time.
             */
            static void LogError(std::string format, ...);

            /**
             * Logs specified message to stdout and log file, if logging
             * to file is enabled in environment settings.
             * Message will be prefixed with WARNING and current time.
             */
            static void LogWarning(std::string format, ...);

    };
}

#define LOG_INFO(fmt, ...) PlainMQ::Logger::LogInfo(fmt, ##__VA_ARGS__);
#define LOG_ERROR(fmt, ...) PlainMQ::Logger::LogError(fmt, ##__VA_ARGS__);
#define LOG_WARNING(fmt, ...) PlainMQ::Logger::LogWarning(fmt, ##__VA_ARGS__);