#include <iostream>
#include <thread>

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
}
