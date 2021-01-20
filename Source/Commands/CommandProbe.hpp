#pragma once

#include "../Common.hpp"
#include "Command.hpp"

namespace PlainMQ {
    class Server;
    class CommandProbe : public PlainMQ::Command {
    public:
        CommandProbe(Server* server);
        
        void OnCommand(Client* client, nlohmann::json data);
    private:
        Server* server;
    };
}