#include "CommandUnsubscribe.hpp"
#include "../Core/Client.hpp"
#include "../Core/Server.hpp"
#include "../Core/Channel.hpp"

using namespace PlainMQ;

CommandUnsubscribe::CommandUnsubscribe(Server* server) {
    this->server = server;
}

void CommandUnsubscribe::OnCommand(Client* client, nlohmann::json data) {
    auto name = data["name"].get<std::string>();
    auto channel = this->server->GetChannelByName(name);
    if (!channel) {
        client->SendError("channel_not_found", name);
        return;
    }

    if (!channel->HasSubscriber(client)) {
        client->SendError("not_subscribed", name);
    } else {
        channel->RemoveSubscriber(client);
    }
}