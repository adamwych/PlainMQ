#pragma once

#include "../Common.hpp"
#include "Command.hpp"

namespace PlainMQ {
    class Server;
    class CommandSubscribe : public PlainMQ::Command {
    public:
        CommandSubscribe(Server* server);
        
        void OnCommand(Client* client, nlohmann::json data);
    private:
        Server* server;
    };
}