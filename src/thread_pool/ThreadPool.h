#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <future>
#include <condition_variable>
#include <mutex>

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();

    template<class F, class... Args>
    auto Enqueue(F &&f, Args &&... args) -> std::future<std::invoke_result_t<F, Args...> > {
        using return_type = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock lock(mtx_);
            if (should_terminate_) {
                throw std::runtime_error("Cannot enqueue on a stopped ThreadPool");
            }
            tasks_.emplace([task]() { (*task)(); });
        }
        cv_.notify_one();
        return res;
    }

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()> > tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool should_terminate_;
    void WorkerThread();
};

#endif
