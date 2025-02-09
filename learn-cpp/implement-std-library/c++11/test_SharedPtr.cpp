#include <iostream>
#include <thread>

#include "shared_ptr.hpp"

using learn_cpp::detail::SharedPtr;

#define SHOW(...) \
    { std::cout << #__VA_ARGS__ " = " << __VA_ARGS__ << '\n'; }

int main() {
    SharedPtr<int> sp1(new int(100));
    SHOW(sp1.get());
}
