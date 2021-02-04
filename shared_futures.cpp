#include <chrono>
#include <future>
#include <iostream>
#include <thread>

// Doesn't work!
// "The state of the promise has already been set"

void foo(std::shared_future<int> fut) {
    std::cout << "foo " + std::to_string(fut.get()) << std::endl;
}

int main() {
    std::promise<int> prom;
    std::shared_future<int> fut = prom.get_future();

    std::thread th1(foo, fut);
    prom.set_value(5);

    std::thread th2(foo, fut);
    prom.set_value(6);

    th1.join();
    th2.join();
}