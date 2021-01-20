#include "./Core/EnvironmentSettings.hpp"
#include "./Core/Server.hpp"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>

PlainMQ::EnvironmentSettings MakeDefaultEnvironmentSettings() {
    PlainMQ::EnvironmentSettings settings;
    settings.hostname = "127.0.0.1";
    settings.port = 9999;
    settings.logToFileEnabled = true;
    settings.logsDirectory = "./logs";
    settings.logLevel = PlainMQ::LogLevel::Default;
    settings.masterKey = "";
    return settings;
}

PlainMQ::EnvironmentSettings ReadEnvironmentSettingsFile(std::string filePath) {
    PlainMQ::EnvironmentSettings settings = MakeDefaultEnvironmentSettings();

    auto stream = std::ifstream(filePath);
    auto contents = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    auto data = nlohmann::json::parse(contents);

    if (data.contains("host"))
        settings.hostname = data["host"];

    if (data.contains("port"))
        settings.port = data["port"];

    if (data.contains("logToFileEnabled"))
        settings.logToFileEnabled = data["logToFileEnabled"];

    if (data.contains("logsDirectory"))
        settings.logsDirectory = data["logsDirectory"];

    if (data.contains("logLevel"))
        settings.logLevel = data["logLevel"].get<std::string>() == "verbose"
            ? PlainMQ::LogLevel::Verbose
            : PlainMQ::LogLevel::Default;

    if (data.contains("reuseAddr"))
        settings.reuseAddr = data["reuseAddr"];

    if (data.contains("masterKey"))
        settings.logsDirectory = data["masterKey"];

    return settings;
}

int main(int argc, char** argv) {
    PlainMQ::EnvironmentSettings settings;

    if (argc >= 2) {
        if (!std::filesystem::exists(argv[1])) {
            LOG_ERROR("Settings file %s was not found.\n", argv[1]);
            return -1;
        }

        settings = ReadEnvironmentSettingsFile(argv[1]);
    } else {
        settings = MakeDefaultEnvironmentSettings();
    }

    PlainMQ::Logger::Initialize(settings);

    PlainMQ::Server* server = new PlainMQ::Server(settings);
    server->Start();
    return 0;
}