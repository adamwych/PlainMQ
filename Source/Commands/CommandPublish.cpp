#include "CommandPublish.hpp"
#include "../Core/Client.hpp"
#include "../Core/Server.hpp"
#include "../Core/Channel.hpp"

using namespace PlainMQ;

typedef struct {
    std::string name;
    std::string password;
} CommandPublishDto;

CommandPublish::CommandPublish(Server* server) {
    this->server = server;
}

void CommandPublish::OnCommand(Client* client, nlohmann::json data) {
    auto name = data["name"];
    auto password = data["password"];
    auto channel = this->server->GetChannelByName(name);
    if (!channel) {
        client->SendError("channel_not_found", name);
        return;
    }

    if (channel->GetPassword().length() > 0 && password != channel->GetPassword()) {
        client->SendError("invalid_password", name);
        return;
    }

    channel->Publish(data["message"]);
}