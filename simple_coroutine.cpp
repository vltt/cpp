#include <experimental/coroutine>
#include <iostream>

// This class is responsible for abstracting the coroutine to the outside world.
// We have a constructor that takes the handle from the promise as per the
// promise's get_return_object implementation.
class my_resumable {
public:
    struct promise_type;

    my_resumable(std::experimental::coroutine_handle<promise_type> handle)
        : handle_(handle) {
        assert(handle);
    }
    my_resumable(my_resumable&) = delete;
    my_resumable(my_resumable&&) = delete;

    bool resume() {
        if (!handle_.done()) {
            handle_.resume();
        }
        return !handle_.done();
    }

    int operator()();

    ~my_resumable() { handle_.destroy(); }

private:
    std::experimental::coroutine_handle<promise_type> handle_;
};

// Why not create abstract class and inherite promises from it?

// The promise is allocate when the coroutine is instantiated and exits for the
// entire lifetime of the coroutine.
// Corrutine returns promise.
struct my_resumable::promise_type {
    // Once done, the compiler calls get_return_object. Returns handle.
    auto get_return_object() noexcept {
        return std::experimental::coroutine_handle<promise_type>::from_promise(
            *this);
    }

    // Once our coroutine is created, we want to run it immediately or remain it
    // suspended.
    // - remain suspended: suspend_always()
    // - start immediately: suspend_never()
    auto initial_suspend() noexcept {
        return std::experimental::suspend_always();
    }

    // What to do before we destroy our coroutine.
    // - To retrieve the result: suspend_always()
    // - Coroutine destroys itself as soon as it done: suspend_never()
    auto final_suspend() noexcept {
        return std::experimental::suspend_always();
    }

    // We need to know what to do when the co_return operator is called in the
    // coroutine. This is done via the return_value function. In this case we
    // store the value in the Promise for later retrieval via the Future.
    // - Also can be return_void()
    void return_value(int val) noexcept { this->val = val; }

    // In the event of an exception we need to know what to do.
    void unhandled_exception() noexcept { std::terminate(); }

    int val;
};

int my_resumable::operator()() {
    if (handle_.done()) {
        return handle_.promise().val;
    } else {
        throw std::runtime_error("not done");
    }
}

my_resumable foo() {
    std::cout << "a" << std::endl;
    co_await std::experimental::suspend_always();
    std::cout << "b" << std::endl;
    co_return 10;
}

int main() {
    my_resumable res = foo();
    res.resume();
    res.resume();
    res.resume();
    res.resume();
    std::cout << res() << std::endl;
}