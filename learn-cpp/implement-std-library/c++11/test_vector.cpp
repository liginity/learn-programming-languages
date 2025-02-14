#include <iostream>

#include "vector.hpp"

void test_v1_vector_1();

int main() {
    test_v1_vector_1();
}

void test_v1_vector_1() {
    using learn_cpp::detail::v1::vector;

    vector<int> vec1{};
    vector<int> vec2(16);
}
