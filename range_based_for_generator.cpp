#include <experimental/coroutine>
#include <iostream>

class Generator {
public:
    struct promise_type;

    struct EndIterator {};

    class GenIterator {
    public:
        GenIterator(std::experimental::coroutine_handle<promise_type>& handle)
            : handle_(handle) {}
        bool operator!=(EndIterator other) { return !handle_.done(); }
        int operator*() const { return handle_.promise().val; }
        GenIterator const& operator++() {
            handle_.resume();
            return *this;
        }

    private:
        std::experimental::coroutine_handle<promise_type> handle_;
    };

    Generator(std::experimental::coroutine_handle<promise_type> handle)
        : handle_(handle) {}
    Generator(const Generator&) = delete;
    Generator(Generator&&) = delete;

    ~Generator() { handle_.destroy(); }

    GenIterator begin() { return handle_; }
    EndIterator end() { return {}; }

    struct promise_type {
        auto get_return_object() noexcept {
            return std::experimental::coroutine_handle<
                promise_type>::from_promise(*this);
        }

        auto initial_suspend() noexcept {
            return std::experimental::suspend_never();
        }

        auto final_suspend() noexcept {
            return std::experimental::suspend_always();
        }

        void return_void() {}

        auto yield_value(int val) noexcept {
            this->val = val;
            return std::experimental::suspend_always();
        };

        void unhandled_exception() noexcept { std::terminate(); }

        int val;
    };

private:
    std::experimental::coroutine_handle<promise_type> handle_;
};

Generator range(int begin, int end) {
    for (int i = begin; i < end; i++) {
        co_yield i;  // co_await promise.yield_value(i)
    }
}

int main() {
    Generator gen = range(3, 15);
    for (auto v : gen) {
        std::cout << v << std::endl;
    }
}