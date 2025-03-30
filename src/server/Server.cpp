#include "Server.h"

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <csignal>

Server::Server(const int port, const size_t number_thread): socket_(port), epoll_(1000), pool_(number_thread) {
    epoll_.AddFd(socket_.GetServerFd(), EPOLLIN | EPOLLET,
                 [this](const int fd, const uint32_t events) { HandleServerEvent(fd, events); });
}

void Server::Start() {
    signal(SIGPIPE, SIG_IGN); // Ignore SIGPIPE
    std::cout << "Server running on port 8080...\n";
    epoll_.Run();
}

void Server::HandleServerEvent(const int fd, const uint32_t events) {
    if (events & EPOLLIN) {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);
        const int client_fd = accept4(fd, reinterpret_cast<sockaddr *>(&client_addr), &len, SOCK_NONBLOCK);
        if (client_fd < 0) {
            return;
        }

        epoll_.AddFd(client_fd, EPOLLIN | EPOLLET | EPOLLONESHOT,
                     [this](const int fd, const uint32_t events) {
                         pool_.Enqueue([this, fd, events] { HandleClientEvent(fd, events); });
                     });
    }
}

void Server::HandleClientEvent(const int fd, const uint32_t events) const {
    if (events & EPOLLIN) {
        char buffer[1024]{};
        ssize_t bytes = read(fd, buffer, sizeof(buffer));
        if (bytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return;
            }
            epoll_.RemoveFd(fd);
            std::cout << "Client disconnected with error: " << fd << " - " << strerror(errno) << "\n";
            return;
        }
        if (bytes == 0) {
            epoll_.RemoveFd(fd);
            std::cout << "Client disconnected: " << fd << "\n";
            return;
        }

        const HttpRequest request{buffer};

        const std::string body = request.GetMethod() == "GET" ? "Hello, World!" : request.GetBody();
        HttpResponse response{request.GetVersion(), 200, body};

        if (write(fd, response.ToString().c_str(), response.ToString().length()) < 0) {
            epoll_.RemoveFd(fd);
            std::cout << "Write failed, client disconnected: " << fd << " - " << strerror(errno) << "\n";
        }
    }
}

