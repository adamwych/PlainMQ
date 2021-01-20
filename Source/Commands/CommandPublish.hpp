#pragma once

#include "../Common.hpp"
#include "Command.hpp"

namespace PlainMQ {
    class Server;
    class CommandPublish : public PlainMQ::Command {
    public:
        CommandPublish(Server* server);
        
        void OnCommand(Client* client, nlohmann::json data);
    private:
        Server* server;
    };
}