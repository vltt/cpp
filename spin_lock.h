#include <atomic>

class spin_lock {
private:
    std::atomic_flag lock_flag_ = ATOMIC_FLAG_INIT;

public:
    bool try_lock() {
        return !lock_flag_.test_and_set(std::memory_order_acquire);
    }

    // test_and_set requires write accesses to cache line, where the lock is stored.
    // If there are many spin_lock's in different threads, each thread will update
    // cashe. To reduse it will "test" lock and try to acquire it only if returned
    // value is false.
    void lock() {
        while (true) {
            if (!lock_flag_.test_and_set(std::memory_order_acquire)) {
                return;
            }
            while (lock_flag_.test(std::memory_order_relaxed))
                ;  // since C++20. For older version use std::atomic<bool> with
                   // "load" funciton (but can be not lock-free) or use
                   // implemetation without cache optimization
                   // spin_lock_not_optimized
        }
    }

    void unlock() { lock_flag_.clear(std::memory_order_release); }
};

class spin_lock_not_optimized {
private:
    std::atomic_flag lock_flag_ = ATOMIC_FLAG_INIT;

public:
    bool try_lock() {
        return !lock_flag_.test_and_set(std::memory_order_acquire);
    }

    void lock() {
        while (lock_flag_.test_and_set(std::memory_order_acquire))
            ;
    }

    void unlock() { lock_flag_.clear(std::memory_order_release); }
};
