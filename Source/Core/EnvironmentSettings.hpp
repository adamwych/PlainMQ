#pragma once

#include "../Common.hpp"

namespace PlainMQ {
    enum LogLevel {
        Default = 0,
        Verbose = 1
    };

    struct EnvironmentSettings {
        std::string hostname;
        uint32_t port;
        std::string logsDirectory;
        bool logToFileEnabled;
        LogLevel logLevel;
        bool reuseAddr;
        std::string masterKey;
    };
}