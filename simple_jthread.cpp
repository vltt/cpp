#include <chrono>
#include <iostream>
#include <thread>

void foo(std::stop_token token, int n) {
    for (int i = 0; i < n; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "foo is  working" + std::to_string(i) << std::endl;
    }
}

void foo_stop(std::stop_token token, int n) {
    for (int i = 0; i < n; i++) {
        if (token.stop_requested()) {
            std::cout << "Stop requested" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "foo_stop is  working" + std::to_string(i) << std::endl;
    }
}

int main() {
    std::jthread jth_stop(foo_stop, 10);
    std::jthread jth(foo, 10);

    jth_stop.request_stop();

    std::cout << "By!" << std::endl;
}