#include "ThreadPool.h"

ThreadPool::ThreadPool(const size_t num_threads) : should_terminate_(false) {
    threads_.reserve(num_threads);
    for (size_t i = 0; i < num_threads; i++) {
        threads_.emplace_back([this] { WorkerThread(); });
    }
}

void ThreadPool::WorkerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock lock(mtx_);
            cv_.wait(lock, [this] {
                return should_terminate_ || !tasks_.empty();
            });

            if (should_terminate_ && tasks_.empty()) {
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard lock(mtx_);
        should_terminate_ = true;
    }
    cv_.notify_all();
    for (auto &thread: threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}