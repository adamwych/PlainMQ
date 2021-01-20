#pragma once

#include "../Common.hpp"
#include <nlohmann/json.hpp>
#include <vector>

namespace PlainMQ {
    class Server;
    class Client;
    class Channel {
    public:
        Channel(std::string name);
        Channel(std::string name, std::string password);

        void PublishText(std::string message);
        void Publish(nlohmann::json data);

        void AddSubscriber(Client* client);
        void RemoveSubscriber(Client* client);
        bool HasSubscriber(Client* client);
    
        std::string GetName();
        std::string GetPassword();

    private:
        std::string name;
        std::string password;
        std::vector<Client*> subscribers;
    };
}