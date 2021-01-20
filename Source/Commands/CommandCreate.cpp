#include "CommandCreate.hpp"
#include "../Core/Client.hpp"
#include "../Core/Server.hpp"
#include "../Core/Channel.hpp"

using namespace PlainMQ;

CommandCreate::CommandCreate(Server* server) {
    this->server = server;
}

void CommandCreate::OnCommand(Client* client, nlohmann::json data) {
    std::string name = data["name"];

    if (this->server->HasChannel(name)) {
        client->SendError("channel_already_exists", name);
        return;
    }

    Channel* channel;

    if (data.contains("password")) {
        channel = new Channel(name, data["password"]);
    } else {
        channel = new Channel(name);
    }

    this->server->AddChannel(name, channel);
}