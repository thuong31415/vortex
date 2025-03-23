#ifndef SOCKET_H
#define SOCKET_H

class Socket {
public:
    explicit Socket(int port);

    ~Socket();

    [[nodiscard]] int AcceptConnection() const;

    [[nodiscard]] int GetServerFd() const;

private:
    int server_fd_;
};

#endif //SOCKET_H
