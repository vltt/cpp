#include <condition_variable>
#include <iostream>
#include <thread>

#include "thread_guard.h"

void foo() { std::cout << "Hello foo!" << std::endl; }

class CallableClass {
public:
    void operator()() { std::cout << "Hello callable_class!" << std::endl; }
};

int main() {
    std::thread th1(foo);

    CallableClass cl;
    std::thread th2(cl);

    std::thread th3([]() { std::cout << "Hello lamda!" << std::endl; });

    th1.join();  // wait until th1 finish
    th2.join();
    th3.join();

    // std::thread th(foo);
    // thread_guard th_g = thread_guard(th);
    // thread_guard th_g1 = thread_guard(th);
    // th_g.join();

    std::cout << "By!" << std::endl;
}