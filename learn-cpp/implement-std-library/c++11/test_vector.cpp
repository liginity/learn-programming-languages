#include <cassert>
#include <iostream>

#include "vector.hpp"

#define SHOW(...) \
    { std::cout << #__VA_ARGS__ " = " << __VA_ARGS__ << std::endl; }

void test_v1_vector_1();

int main() { test_v1_vector_1(); }

void test_v1_vector_1() {
    using learn_cpp::detail::v1::vector;

    vector<int> vec1{};
    vector<int> vec2(16);
    for (auto item : vec2) {
        assert(item == 0);
    }

    vector<int> vec3 = {1, 2, 3, 4};
    SHOW(vec3.size());
    SHOW(vec3.capacity());
    for (auto item : vec3) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
}
