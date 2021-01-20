#include "CommandSubscribe.hpp"
#include "../Core/Client.hpp"
#include "../Core/Server.hpp"
#include "../Core/Channel.hpp"

using namespace PlainMQ;

CommandSubscribe::CommandSubscribe(Server* server) {
    this->server = server;
}

void CommandSubscribe::OnCommand(Client* client, nlohmann::json data) {
    auto name = data["name"].get<std::string>();
    auto channel = this->server->GetChannelByName(name);
    if (!channel) {
        client->SendError("channel_not_found", name);
        return;
    }

    if (channel->HasSubscriber(client)) {
        client->SendError("already_subscribed", name);
    } else {
        channel->AddSubscriber(client);
    }
}