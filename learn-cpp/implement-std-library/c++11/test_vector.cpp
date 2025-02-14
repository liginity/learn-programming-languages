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

    unsigned size3 = 16;
    const vector<int> vec3(size3, 7);
    assert(vec3.size() == size3);
    for (int i = 0; i < size3; ++i) {
        assert(vec3[i] == 7);
    }

    auto vec4 = vec3;
    assert(vec4.size() == vec3.size());

    vec4.push_back(9);
    auto vec5 = std::move(vec4);
    auto size5 = size3 + 1;
    assert(vec5.size() == size5);

    vector<int> vec6 = {1, 2, 3, 4};
    SHOW(vec6.size());
    SHOW(vec6.capacity());
    for (auto item : vec6) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
}
