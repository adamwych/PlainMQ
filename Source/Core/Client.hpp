#pragma once

#include <nlohmann/json.hpp>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/common/connection_hdl.hpp>

typedef websocketpp::server<websocketpp::config::asio> WebSocketServer;

using json = nlohmann::json;

namespace PlainMQ {
    class Server;
    class Client {
    public:
        Client(websocketpp::connection_hdl hdl, WebSocketServer::connection_ptr connection);

        /**
         * Sends given text message to the client.
         */
        void SendText(const std::string& message);

        void SendError(const std::string& message, const std::string& context = "");

        /**
         * Converts given JSON object to text and then sends
         * it to the client.
         */
        void Send(const json& data);

        /**
         * Returns the full address of this client in "hostname:port" format.
         */
        std::string GetAddress();

        WebSocketServer::connection_ptr GetConnection();
        websocketpp::connection_hdl GetHandle();
    private:
        WebSocketServer::connection_ptr connection;
        websocketpp::connection_hdl hdl;
    };
}