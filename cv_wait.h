#include <concepts>
#include <mutex>

void wait(std::unique_lock<std::mutex>& ul, Predicate cond) {
    while (true) {
        if (!ul.owns_lock()) {
            ul.lock();
        }
        if (cond() == true) {
            return;
        } else {
            ul.unlock();
            sleep_until_notify();
        }
    }
}