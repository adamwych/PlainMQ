#include "MessageHandler.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "../Commands/Command.hpp"
#include "../Commands/CommandCreate.hpp"
#include "../Commands/CommandSubscribe.hpp"
#include "../Commands/CommandPublish.hpp"
#include "../Commands/CommandUnsubscribe.hpp"
#include "../Commands/CommandProbe.hpp"

#include <nlohmann/json.hpp>

using namespace PlainMQ;
using json = nlohmann::json;

MessageHandler::MessageHandler(Server* server) {
    this->server = server;
}

void MessageHandler::OnMessage(Client* client, const std::string& payload) {
    try {
        auto data = json::parse(payload.c_str());
        if (data.contains("command")) {
            auto commandName = data["command"];
            auto commandHandler = GetCommandHandler(commandName);

            if (commandHandler) {
                commandHandler->OnCommand(client, data);
            } else {
                client->SendError("unknown_command");
            }
        } else {
            client->SendError("unknown_command");
        }
    } catch (json::exception ex) {
        client->SendError("unprocessable_message");
    }
}

Command* MessageHandler::GetCommandHandler(std::string commandName) {
    if (commandName == "create") {
        return new CommandCreate(this->server);
    } else if (commandName == "subscribe") {
        return new CommandSubscribe(this->server);
    } else if (commandName == "publish") {
        return new CommandPublish(this->server);
    } else if (commandName == "unsubscribe") {
        return new CommandUnsubscribe(this->server);
    } else if (commandName == "probe") {
        return new CommandProbe(this->server);
    }

    return nullptr;
}