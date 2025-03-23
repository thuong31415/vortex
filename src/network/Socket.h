#ifndef SOCKET_H
#define SOCKET_H

class Socket {
public:
    Socket();

    ~Socket();

    void InitSocket(int port);

    [[nodiscard]] int Accept() const;

private:
    void Create();

    void Bind(int port);

    void Listen() const;

    int server_fd_;

    int port_{};
};

#endif //SOCKET_H
