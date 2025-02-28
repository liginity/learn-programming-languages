#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <type_traits>

#include "compressed_pair.hpp"

using learn_cpp::detail::CompressedPair;

class Base1 {};

void test1() {
    // EBO
    // std::allocator<T> is an empty type, because it is stateless.
    assert(sizeof(CompressedPair<int, std::allocator<int>>) == sizeof(int));

    CompressedPair<int, std::allocator<int>> pair1;
    pair1.first() = 10;
    assert(pair1.first() == 10);
    static_assert(
        std::is_same<std::remove_reference<decltype(pair1.second())>::type,
                     std::allocator<int>>::value,
        "expect same type, but get different types");
    auto alloc = pair1.second();
    auto p1 = alloc.allocate(1);
    alloc.construct(p1, 20);
    assert(*p1 == 20);
    alloc.destroy(p1);
    alloc.deallocate(p1, 1);

    // no EBO
    assert(sizeof(CompressedPair<std::int64_t, double>) ==
           sizeof(std::int64_t) + sizeof(double));
}

int main() {
    //
    test1();
}
