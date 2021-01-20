#include "CommandProbe.hpp"
#include "../Core/Client.hpp"
#include "../Core/Server.hpp"
#include "../Core/Channel.hpp"
#include "../Version.hpp"

using namespace PlainMQ;

CommandProbe::CommandProbe(Server* server) {
    this->server = server;
}

void CommandProbe::OnCommand(Client* client, nlohmann::json data) {
    if (!data.contains("key")) {
        client->SendError("invalid_master_key");
        return;
    }

    std::string key = data["key"];
    if (key == this->server->GetSettings().masterKey) {
        client->Send(json({
            {"version_major", PLAINMQ_MAJOR_VERSION},
            {"version_minor", PLAINMQ_MINOR_VERSION},
            {"version_patch", PLAINMQ_PATCH_VERSION},
            {"connected_clients", this->server->GetClients().size()},
            {"active_channels", this->server->GetChannels().size()},
        }));
    } else {
        client->SendError("invalid_master_key");
    }
}