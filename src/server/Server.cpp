#include "Server.h"

#include <iostream>
#include <unistd.h>

#include "../http/HttpHandler.h"

Server::Server(const int port): socket_(port), epoll_(1000) {
    epoll_.AddFd(socket_.GetServerFd(), EPOLLIN | EPOLLET,
                 [this](const int fd, const uint32_t events) { handleServerEvent(fd, events); });
}

void Server::Start() {
    std::cout << "Server running on port 8080...\n";
    epoll_.Run();
}

void Server::handleServerEvent(const int fd, const uint32_t events) {
    if (events & EPOLLIN) {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);
        const int client_fd = accept4(fd, reinterpret_cast<sockaddr *>(&client_addr), &len, SOCK_NONBLOCK);
        if (client_fd < 0) {
            return;
        }
        epoll_.AddFd(client_fd, EPOLLIN | EPOLLET,
                     [this](const int fd, const uint32_t events) { handleClientEvent(fd, events); });
    }
}

void Server::handleClientEvent(const int fd, const uint32_t events) const {
    if (events & EPOLLIN) {
        char buffer[1024]{};
        const ssize_t bytes = read(fd, buffer, sizeof(buffer));
        if (bytes <= 0) {
            epoll_.RemoveFd(fd);
            std::cout << "Client disconnected: " << fd << "\n";
            return;
        }

        const std::string request(buffer, bytes);
        std::cout << "Received: " << request;

        const std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
        write(fd, response.c_str(), response.length());
        epoll_.RemoveFd(fd);
        std::cout << "Response sent to client: " << fd << "\n";
    }
}
