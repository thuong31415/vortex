#ifndef EPOLL_H
#define EPOLL_H

#include <functional>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <vector>
#include <shared_mutex>

class Epoll {
public:
    explicit Epoll(int max_events = 1000);

    ~Epoll();

    // Non-copyable and non-movable
    Epoll(const Epoll&) = delete;
    Epoll& operator=(const Epoll&) = delete;
    Epoll(Epoll&&) = delete;
    Epoll& operator=(Epoll&&) = delete;


    using EventCallback = std::function<void(int fd, uint32_t events)>;

    void AddFd(int fd, uint32_t events, EventCallback callback);

    void ModifyFd(int fd, uint32_t events) const;

    void RemoveFd(int fd);

    void Run();

    static void SetNonBlocking(int fd);

private:
    int epoll_fd_;
    int max_events_;
    std::vector<epoll_event> events_;

    std::shared_mutex callbacks_mutex_;
    std::unordered_map<int, EventCallback> callbacks_;
};

#endif
