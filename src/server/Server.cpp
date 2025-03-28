#include "Server.h"

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <csignal>

Server::Server(const int port, const size_t number_thread): socket_(port), epoll_(1000), pool_(number_thread) {
    epoll_.AddFd(socket_.GetServerFd(), EPOLLIN | EPOLLET,
                 [this](const int fd, const uint32_t events) { handleServerEvent(fd, events); });
}

void Server::Start() {
    signal(SIGPIPE, SIG_IGN); // Ignore SIGPIPE
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

        epoll_.AddFd(client_fd, EPOLLIN | EPOLLET ,
                     [this](const int fd, const uint32_t events) {
                         pool_.Enqueue([this, fd, events] { handleClientEvent(fd, events); });
                     });
    }
}

void Server::handleClientEvent(const int fd, const uint32_t events) const {
    if (events & EPOLLIN) {
        char buffer[1024]{};

        if (const ssize_t bytes = read(fd, buffer, sizeof(buffer)); bytes <= 0) {
            epoll_.RemoveFd(fd);
            std::cout << "Client disconnected: " << fd << "\n";
            return;
        }

        const HttpRequest request{buffer};
        const HttpResponse response{request.GetVersion(), 200, request.GetBody()};

        if (write(fd, response.ToString().c_str(), response.ToString().length()) < 0) {
            if (errno == EPIPE) {
                std::cout << "Client closed connection: " << fd << "\n";
            } else {
                std::cerr << "Write error on fd " << fd << ": " << strerror(errno) << "\n";
            }
            epoll_.RemoveFd(fd);
        }

    }
}
