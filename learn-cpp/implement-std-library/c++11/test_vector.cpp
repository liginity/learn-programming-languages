#include <cassert>
#include <iostream>
#include <string>

#include "vector.hpp"

#define SHOW(...) \
    { std::cout << #__VA_ARGS__ " = " << __VA_ARGS__ << std::endl; }

void test_v1_vector_1();
void test_v1_vector_2();

int main() {
    test_v1_vector_1();
    test_v1_vector_2();
}

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

void test_v1_vector_2() {
    using learn_cpp::detail::v1::vector;
    using std::string;

    vector<string> vec1{};
    unsigned size2 = 8;
    vector<string> vec2(size2);

    unsigned size3 = 5;
    const char* cstr3 = "abc";
    vector<string> vec3(size3, cstr3);
    for (decltype(size3) i = 0; i < size3; ++i) {
        assert(vec3[i] == cstr3);
    }

    // copy ctor
    auto vec4 = vec3;
    vec4.emplace_back(4, 'a');
    assert(*(vec4.end() - 1) == "aaaa");

    // move ctor
    auto vec5 = std::move(vec4);
    unsigned size5 = size3 + 1;
    assert(vec5.size() == size5);
}
