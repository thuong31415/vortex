#ifndef SERVER_H
#define SERVER_H

#include "../network/Socket.h"
#include "../network/Epoll.h"

class Server {
public:
    explicit Server(int port);

    ~Server();

    [[noreturn]] void Run();

private:
    Socket socket_;
    Epoll epoll_;
};


#endif //SERVER_H
