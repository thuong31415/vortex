#include "Socket.h"

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

Socket::Socket(const int port) {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        throw std::runtime_error("Could not create socket");
    }

    constexpr int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("set sock opt SO_REUSEADDR failed");
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_fd_, reinterpret_cast<sockaddr *>(&server_address), sizeof(server_address)) < 0) {
        throw std::runtime_error("bind socket error");
    }

    if (listen(server_fd_, SOMAXCONN) == -1) {
        throw std::runtime_error("Failed to listen on socket");
    }
}

Socket::~Socket() {
    if (server_fd_ != -1) {
        close(server_fd_);
    }
}

[[nodiscard]] int Socket::AcceptConnection() const {
    sockaddr_in client_address{};
    socklen_t client_addr_len = sizeof(client_address);
    const int client_fd = accept(server_fd_, reinterpret_cast<sockaddr *>(&client_address), &client_addr_len);
    if (client_fd < 0) {
        throw std::runtime_error("accept socket error");
    }
    return client_fd;
}

int Socket::GetServerFd() const {
    return server_fd_;
}
