#pragma once

#include <thread>

class thread_guard {
private:
    std::thread& th;

public:
    explicit thread_guard(std::thread& th) : th(th){};

    void join() { th.join(); }

    ~thread_guard() {
        if (th.joinable()) {
            th.join();
        }
    }

    thread_guard(thread_guard&) = delete;
    thread_guard& operator=(thread_guard&) = delete;
};
