#pragma once

#include "../Common.hpp"
#include "Command.hpp"

namespace PlainMQ {
    class Server;
    class CommandUnsubscribe : public PlainMQ::Command {
    public:
        CommandUnsubscribe(Server* server);
        
        void OnCommand(Client* client, nlohmann::json data);
    private:
        Server* server;
    };
}