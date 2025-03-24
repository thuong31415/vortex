#ifndef EPOLL_H
#define EPOLL_H

#include <functional>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <vector>

class Epoll {
public:
    explicit Epoll(int max_events = 1000);

    ~Epoll();

    using EventCallback = std::function<void(int fd, uint32_t events)>;

    void AddFd(int fd, uint32_t events, EventCallback callback);

    void ModifyFd(int fd, uint32_t events) const;

    void RemoveFd(int fd) const;

    void Run();

    static void SetNonBlocking(int fd);

private:
    int epoll_fd_;
    int max_events_;
    std::vector<epoll_event> events_;
    std::unordered_map<int, EventCallback> callbacks_;
};

#endif
