#pragma once

#include "../Common.hpp"
#include <nlohmann/json.hpp>

namespace PlainMQ {
    class Client;
    class Command {
    public:
        virtual void OnCommand(Client* client, nlohmann::json data) = 0;
    };
}