#include "Channel.hpp"
#include "Client.hpp"

using namespace PlainMQ;

Channel::Channel(std::string name) {
    this->name = name;
    this->password = "";
}

Channel::Channel(std::string name, std::string password) {
    this->name = name;
    this->password = password;
}

void Channel::PublishText(std::string message) {
    for (auto& client : this->subscribers) {
        client->Send(json({
            {"type", "channel_message"},
            {"message", message}
        }));
    }
}

void Channel::Publish(nlohmann::json data) {
    for (auto& client : this->subscribers) {
        client->Send(json({
            {"type", "channel_message"},
            {"message", data}
        }));
    }
}

void Channel::AddSubscriber(Client* client) {
    this->subscribers.push_back(client);
}

void Channel::RemoveSubscriber(Client* client) {
    auto s = std::find(this->subscribers.begin(), this->subscribers.end(), client);
    if (s != this->subscribers.end()) {
        this->subscribers.erase(s);
    }
}

bool Channel::HasSubscriber(Client* client) {
    return std::find(this->subscribers.begin(), this->subscribers.end(), client) != this->subscribers.end();
}

std::string Channel::GetName() {
    return this->name;
}

std::string Channel::GetPassword() {
    return this->password;
}