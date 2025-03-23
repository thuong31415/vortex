#ifndef SERVER_H
#define SERVER_H

#include "../network/Socket.h"

class Server {
public:
    Server();

    ~Server();

    [[noreturn]] void Run(int port);

private:
    Socket socket_{};
};


#endif //SERVER_H
