#include "Server.h"
#include "../http/HttpHandler.h"

Server::Server(const int port): socket_(port), epoll_(1000) {
    epoll_.AddFd(socket_.GetServerFd(), EPOLLIN);
    Epoll::SetNonBlocking(socket_.GetServerFd());
}

Server::~Server() = default;

[[noreturn]] void Server::Run() {
    while (true) {
        const int n = epoll_.Poll(-1);
        for (int i = 0; i < n; i++) {
            epoll_event &event = epoll_.GetEvent(i);
            if (event.data.fd == socket_.GetServerFd()) {
                const int client_fd = socket_.AcceptConnection();
                epoll_.AddFd(client_fd, EPOLLIN);
            } else {
                HttpHandler::HandleClient(event.data.fd);
            }
        }
    }
}
