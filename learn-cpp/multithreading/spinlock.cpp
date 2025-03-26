#include "spinlock.hpp"

#include <atomic>

Spinlock::Spinlock() {}

Spinlock::~Spinlock() {}

void Spinlock::Lock() {
    while (flag_.test_and_set() == true) {
        // busy looping
    }
}

void Spinlock::Unlock() {
    flag_.clear();
}
