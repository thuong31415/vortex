#include "Server.h"
#include "../http/HttpHandler.h"

Server::Server() = default;
Server::~Server() = default;


[[noreturn]] void Server::Run(const int port) {
    socket_.InitSocket(port);
    while (true) {
        const int client_fd = socket_.Accept();
        HttpHandler::HandleClient(client_fd);
    }
}
