#include "Server.hpp"
#include "EnvironmentSettings.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "MessageHandler.hpp"
#include "../Version.hpp"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using namespace PlainMQ;

Server::Server(const EnvironmentSettings& settings) {        
    this->settings = settings;
    this->messageHandler = new MessageHandler(this);
}

Server::~Server() {
    for (auto& client : this->clients) {
        this->server->close(client.first, websocketpp::close::status::going_away, "Server closed");
        delete client.second;
    }
}

void Server::LogWelcomeMessage() {
    LOG_INFO(
        "Starting PlainMQ server %d.%d.%d at %s:%d",
        PLAINMQ_MAJOR_VERSION,
        PLAINMQ_MINOR_VERSION,
        PLAINMQ_PATCH_VERSION,
        this->settings.hostname.c_str(),
        this->settings.port
    );

    LOG_INFO("Environment settings:");
    LOG_INFO("  Hostname = %s", this->settings.hostname.c_str());
    LOG_INFO("  Port = %d", this->settings.port);
    LOG_INFO("  Logs directory = %s", this->settings.logsDirectory.c_str());
    LOG_INFO("  Logging to file enabled? %s", this->settings.logToFileEnabled ? "Yes" : "No");
}

void Server::Start() {
    this->LogWelcomeMessage();

    this->server = new WebSocketServer();

    if (this->settings.reuseAddr) {
        this->server->set_reuse_addr(true);
    }

    this->server->set_access_channels(websocketpp::log::alevel::none);
    this->server->init_asio();

    this->server->set_open_handler(bind(&Server::OnConnectionOpen, this, ::_1));
    this->server->set_close_handler(bind(&Server::OnConnectionClose, this, ::_1));
    this->server->set_message_handler(bind(&Server::OnMessage, this, ::_1, ::_2));

    this->server->listen(this->settings.port);

    this->server->start_accept();
    this->server->run();
}

void Server::OnConnectionOpen(websocketpp::connection_hdl hdl) {
    auto connection = this->server->get_con_from_hdl(hdl);
    auto client = new Client(hdl, connection);
    
    if (this->settings.logLevel >= LogLevel::Verbose) {
        LOG_INFO("New connection from %s", client->GetAddress().c_str());
    }

    this->clients[hdl] = client;
}

void Server::OnConnectionClose(websocketpp::connection_hdl hdl) {
    auto connection = this->server->get_con_from_hdl(hdl);
    auto client = this->clients[hdl];
    
    if (client && this->settings.logLevel >= LogLevel::Verbose) {
        LOG_INFO("Connection closed: %s", client->GetAddress().c_str());
    }

    delete client;
    this->clients.erase(hdl);
}

void Server::OnMessage(websocketpp::connection_hdl hdl, WebSocketServer::message_ptr message) {
    auto client = this->clients[hdl];
    if (client) {
        messageHandler->OnMessage(client, message->get_payload());
    } else {
        LOG_WARNING("Received a message from an unknown client. Ignoring.");
    }
}

void Server::AddChannel(std::string name, Channel* channel) {
    this->channels[name] = channel;
}

bool Server::HasChannel(std::string channelName) {
    return this->channels.find(channelName) != this->channels.end();
}

Channel* Server::GetChannelByName(std::string channelName) {
    if (!this->HasChannel(channelName)) {
        return nullptr;
    }

    return this->channels.at(channelName);
}

ServerChannelsMap Server::GetChannels() {
    return this->channels;
}

WebSocketServer* Server::GetSocketServer() {
    return this->server;
}

PlainMQ::ServerClientsMap Server::GetClients() {
    return this->clients;
}

EnvironmentSettings Server::GetSettings() {
    return this->settings;
}