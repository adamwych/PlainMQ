#pragma once

#include "../Common.hpp"

namespace PlainMQ {
    class Server;
    class Client;
    class Command;

    class MessageHandler {
    public:

        MessageHandler(Server* server);

        /**
         * Acts on a message.
         * @param client Client, that sent the message.
         * @param payload The message.
         */
        void OnMessage(Client* client, const std::string& payload);

    private:
        Command* GetCommandHandler(std::string commandName);

        Server* server;
    };
}