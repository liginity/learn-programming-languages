#include <cassert>
#include <iostream>
#include <thread>

#include "spinlock.hpp"

/**
 * One line compilation
 *     g++ -o test_spinlock.out test_spinlock.cpp spinlock.cpp
 */

Spinlock spinlock;
int count1{};

void AddCount(int n) {
    for (int i = 0; i < n; ++i) {
        spinlock.Lock();
        ++count1;
        spinlock.Unlock();
    }
}

int main() {
    int n1 = 10000;
    int n2 = 20000;
    std::thread t1(AddCount, n1);
    std::thread t2(AddCount, n2);
    t1.join();
    t2.join();

    assert(n1 + n2 == count1);
}
