#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    const auto server_ip = "127.0.0.1";
    constexpr int server_port = 8080;

    const int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket\n";
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(sock, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to server\n";
        close(sock);
        return -1;
    }

    const auto request = "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: keep-alive\r\n\r\n";

    send(sock, request, strlen(request), 0);
    std::cout << "Sent request to server\n";

    char buffer[1024] = {};

    if (const ssize_t bytes = read(sock, buffer, sizeof(buffer)); bytes > 0) {
        std::cout << "Received from server:\n" << buffer << "\n";
    }

    std::cout << "Closing connection...\n";
    close(sock);
    return 0;
}
