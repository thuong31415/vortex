#ifndef SERVER_H
#define SERVER_H

#include "../http/HttpRequest.h"
#include "../http/HttpResponse.h"
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
    HttpRequest http_request_{};
    HttpResponse http_response_{};
};


#endif //SERVER_H
