#pragma once

#include "../Common.hpp"
#include "EnvironmentSettings.hpp"
#include <map>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/common/connection_hdl.hpp>

typedef websocketpp::server<websocketpp::config::asio> WebSocketServer;

namespace PlainMQ {
    class Client;
    class Channel;
    class MessageHandler;

    typedef std::map<websocketpp::connection_hdl, Client*, std::owner_less<websocketpp::connection_hdl>> ServerClientsMap;
    typedef std::map<std::string, Channel*> ServerChannelsMap;

    class Server {
        public:
            Server(const EnvironmentSettings& settings);
            ~Server();

            /**
             * Starts the server, if it is not running already.
             */
            void Start();

            void AddChannel(std::string name, Channel* channel);
            Channel* GetChannelByName(std::string channelName);
            bool HasChannel(std::string channelName);
            ServerChannelsMap GetChannels();

            /**
             * Returns the native websocketpp server instance.
             */
            WebSocketServer* GetSocketServer();

            /**
             * Returns a map of all clients connected to the server and their
             * websocketpp connection handles.
             */
            ServerClientsMap GetClients();

            EnvironmentSettings GetSettings();
            
        private:
            void LogWelcomeMessage();

            void OnConnectionOpen(websocketpp::connection_hdl hdl);
            void OnConnectionClose(websocketpp::connection_hdl hdl);
            void OnMessage(websocketpp::connection_hdl hdl, WebSocketServer::message_ptr message);

            ServerChannelsMap channels;

            EnvironmentSettings settings;
            WebSocketServer* server;
            MessageHandler* messageHandler;
            ServerClientsMap clients;
    };
}