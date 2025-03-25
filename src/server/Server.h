#ifndef SERVER_H
#define SERVER_H

#include "../network/Socket.h"
#include "../network/Epoll.h"
#include "../thread_pool/ThreadPool.h"

class Server {
public:
    explicit Server(int port, size_t number_thread);

    void Start();

private:
    void handleServerEvent(int fd, uint32_t events);

    void handleClientEvent(int fd, uint32_t events) const;

    Socket socket_;
    Epoll epoll_;
    ThreadPool pool_;
};


#endif //SERVER_H
