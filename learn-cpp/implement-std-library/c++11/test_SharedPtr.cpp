#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "shared_ptr.hpp"

using learn_cpp::detail::SharedPtr;

#define COUT(str) \
    { std::cout << '\n' << str << '\n'; }

#define SHOW(...) \
    { std::cout << #__VA_ARGS__ " = " << __VA_ARGS__ << '\n'; }

int main() {
    SharedPtr<int> sp1(new int(100));
    COUT("create sp1");
    SHOW(sp1.get());
    SHOW(sp1.use_count());

    auto sp2 = sp1;
    COUT("copy construct sp2 from sp1");
    SHOW(sp1.use_count());
    SHOW(sp2.use_count());

    sp2.reset();
    COUT("reset sp2");
    SHOW(sp1.use_count());
    SHOW(sp2.use_count());

    constexpr int N = 100;
    std::vector<std::thread> threads;

    std::shared_ptr<int> stdsp(new int(1));
    for (int i = 0; i < N; ++i) {
        auto fn = [stdsp]() mutable {
            stdsp.reset();
        };
        threads.emplace_back(fn);
    }
    for (auto &t : threads) {
        t.join();
    }
    threads.clear();
    // std::exit(0);


    for (int i = 0; i < N; ++i) {
        auto fn = [sp1]() mutable {
            sp1.reset();
        };
        threads.emplace_back(fn);
    }
    for (auto &t : threads) {
        t.join();
    }
}
