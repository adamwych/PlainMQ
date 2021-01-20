#include "Client.hpp"
#include "Server.hpp"

using namespace PlainMQ;

Client::Client(websocketpp::connection_hdl hdl, WebSocketServer::connection_ptr connection) {
    this->hdl = hdl;
    this->connection = connection;
}

void Client::SendText(const std::string& message) {
    this->connection->send(message, websocketpp::frame::opcode::TEXT);
}

void Client::SendError(const std::string& message, const std::string& context) {
    Send(json({
        {"type", "error"},
        {"error", message},
        {"context", context}
    }));
}

void Client::Send(const json& data) {
    SendText(data.dump());
}

std::string Client::GetAddress() {
    return this->connection->get_host() + ":" + std::to_string(this->connection->get_port());
}

WebSocketServer::connection_ptr Client::GetConnection() {
    return this->connection;
}

websocketpp::connection_hdl Client::GetHandle() {
    return this->hdl;
}