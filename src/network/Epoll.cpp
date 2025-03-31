#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include "Epoll.h"

#include <cstring>
#include <iostream>
#include <mutex>

Epoll::Epoll(const int max_events) : max_events_(max_events) {
    epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
    if (epoll_fd_ == -1) {
        throw std::runtime_error("Failed to create epoll instance");
    }
    // Pre-allocate memory for events
    events_.resize(max_events);
}

Epoll::~Epoll() {
    if (epoll_fd_ != -1) {
        close(epoll_fd_);
    }
}

void Epoll::SetNonBlocking(const int fd) {
    const int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error("Failed to get fd flags");
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("Failed to set non-blocking");
    }
}

void Epoll::AddFd(const int fd, const uint32_t events, EventCallback callback) {
    SetNonBlocking(fd);

    epoll_event event{};
    event.data.fd = fd;
    event.events = events;

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        throw std::runtime_error("Failed to add fd to epoll");
    }

    std::unique_lock lock(callbacks_mutex_);
    callbacks_[fd] = std::move(callback);
}

void Epoll::ModifyFd(const int fd, const uint32_t events) const {
    epoll_event ev{};
    ev.data.fd = fd;
    ev.events = events;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev) < 0) {
        throw std::runtime_error("Failed to modify fd in epoll");
    }
}

void Epoll::RemoveFd(const int fd) {

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr) < 0 && errno != EBADF) {
        std::cerr << "Warning: Failed to remove fd from epoll: " << strerror(errno) << std::endl;
    }

    shutdown(fd, SHUT_RDWR);
    close(fd);

    std::unique_lock lock(callbacks_mutex_);
    callbacks_.erase(fd);

}

void Epoll::Run() {

    while (true) {
        const int n = epoll_wait(epoll_fd_, events_.data(), max_events_, -1);

        if (n < 0) {
            if (errno == EINTR) {
                // Interrupted system call, just continue
                continue;
            }
            throw std::runtime_error("epoll_wait failed: " + std::string(strerror(errno)));
        }

        for (int i = 0; i < n; ++i) {
            int fd = events_[i].data.fd;
            const uint32_t event_flags = events_[i].events;

            EventCallback callback;
            {
                std::shared_lock lock(callbacks_mutex_);
                if (auto it = callbacks_.find(fd); it != callbacks_.end()) {
                    callback = it->second;
                }
            }

            if (callback) {
                callback(fd, event_flags);
            }
        }
    }
}


