#ifndef SPINLOCK_HPP
#define SPINLOCK_HPP

#include <atomic>

/**
 * A spinlock implemented with C++11 std::atomic_flag.
 * std::atomic_flag is guaranteed to be lock-free.
 */
class Spinlock {
   public:
    Spinlock();
    ~Spinlock();

    // not copyable
    Spinlock(const Spinlock &) = delete;
    Spinlock &operator=(const Spinlock &) = delete;
    // not movable
    Spinlock(const Spinlock &&) = delete;
    Spinlock &operator=(const Spinlock &&) = delete;

    void Lock();
    void Unlock();

   private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};
#endif
