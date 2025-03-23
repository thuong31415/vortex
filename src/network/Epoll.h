#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <netinet/in.h>
#include <vector>

class Epoll {
public:
    explicit Epoll(int max_events);

    ~Epoll();

    void AddFd(int fd, uint32_t events) const;

    void RemoveFd(int fd) const;

    int Poll(int timeout);

    epoll_event &GetEvent(int index);

    static void SetNonBlocking(int fd);

private:
    int epoll_fd_;
    int max_events_;
    std::vector<epoll_event> events_;
};

#endif
