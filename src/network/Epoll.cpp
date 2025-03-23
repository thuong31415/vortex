#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include "Epoll.h"

Epoll::Epoll(const int max_events) : max_events_(max_events) {
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1) {
        throw std::runtime_error("Failed to create epoll instance");
    }
    events_.reserve(max_events);
}

Epoll::~Epoll() {
    close(epoll_fd_);
}

void Epoll::SetNonBlocking(const int fd) {
    const int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void Epoll::AddFd(const int fd, const uint32_t events) const {
    SetNonBlocking(fd);
    epoll_event event{};
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        throw std::runtime_error("Failed to add fd to epoll");
    }
}


void Epoll::RemoveFd(const int fd) const {
    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
    close(fd);
}

int Epoll::Poll(const int timeout) {
    return epoll_wait(epoll_fd_, events_.data(), max_events_, timeout);
}

epoll_event &Epoll::GetEvent(const int index) {
    return events_[index];
}
