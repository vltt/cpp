#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int multiply(int a, int& b, int&& c) {
    std::cout << "Hello multiply!" << std::endl;
    b++;
    return a * b * c;
}

void do_other_operation() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Hello do_other_operation!" << std::endl;
}

int main() {
    int b = 19;
    std::future<int> fut =
        std::async(std::launch::async | std::launch::deferred, multiply, 10,
                   std::ref(b), 30);
    do_other_operation();

    std::cout << fut.get() << std::endl;  // 6000
    std::cout << b << std::endl;          // 20
}