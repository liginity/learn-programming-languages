#include <iostream>
#include <type_traits>
#include <vector>

/**
   NOTE
   Manually implement these facilities:
   - Integral --- std::integral
   - void_t --- std::void_t (c++17)

   Implement a type trait, IsContainer, that checks whether a type has a begin()
   method.

   Add operator<<(std::ostream &, Container).
   Print 1d container and 2d container differently.
 */

// like std::integral
template <typename Type, Type val>
struct Integral {
    using type = Type;
    static constexpr Type value = val;
};

using TrueType = Integral<bool, true>;
using FalseType = Integral<bool, false>;

// like std::void_t in c++17
template <typename...>
using void_t = void;

template <typename Container, typename = void>
struct IsContainer : FalseType {};

// template <typename Container>
// struct IsContainer<Container, void_t<typename Container::begin>> : TrueType
// {};

template <typename Container>
struct IsContainer<Container,
                   void_t<decltype(std::declval<Container>().begin())>>
    : TrueType {};

template <bool b, typename T = void>
using EnableIf = std::enable_if<b, T>;

// print container

// template <typename Container,
//           typename = std::enable_if<IsContainer<Container>::value>>
// std::ostream &operator<<(std::ostream &os, const Container &container) {
//     os << "{ ";
//     for (auto it1 = container.begin(); it1 != container.end(); ++it1) {
//         os << *it1 << ", ";
//     }
//     os << "}";
//     return os;
// }

// template <typename Container>
// std::ostream &operator<<(
//     std::ostream &os,
//     const typename std::enable_if<IsContainer<Container>::value,
//                                   Container>::type &container) {
//     os << "{ ";
//     for (auto it1 = container.begin(); it1 != container.end(); ++it1) {
//         os << *it1 << ", ";
//     }
//     os << "}";
//     return os;
// }

// working solution 1
// clang-format off
// template <typename Container>
// typename std::enable_if<IsContainer<Container>::value, std::ostream>::type &
// operator<<(std::ostream &os, const Container &container) {
//     os << "{ ";
//     for (auto it1 = container.begin(); it1 != container.end(); ++it1) {
//         os << *it1 << ", ";
//     }
//     os << "}";
//     return os;
// }
// clang-format on

// working solution 2
// clang-format off
// template <typename Container,
//           typename std::enable_if<IsContainer<Container>::value, int>::type = 0>
// std::ostream &operator<<(std::ostream &os, const Container &container) {
//     os << "{ ";
//     for (auto it1 = container.begin(); it1 != container.end(); ++it1) {
//         os << *it1 << ", ";
//     }
//     os << "}";
//     return os;
// }
// clang-format on

// NOTE add more constraint to avoid ambiguity.
template <typename Container,
          typename std::enable_if<
              IsContainer<Container>::value &&
                  !IsContainer<typename Container::value_type>::value,
              int>::type = 0>
std::ostream &operator<<(std::ostream &os, const Container &container) {
    os << "{ ";
    for (auto it1 = container.begin(); it1 != container.end(); ++it1) {
        os << *it1 << ", ";
    }
    os << "}";
    return os;
}

// print 2d container
template <typename Container,
          typename std::enable_if<
              IsContainer<Container>::value &&
                  IsContainer<typename Container::value_type>::value,
              int>::type = 0>
std::ostream &operator<<(std::ostream &os, const Container &container) {
    os << "{\n";
    for (auto it1 = container.begin(); it1 != container.end(); ++it1) {
        os << "    " << *it1 << ",\n";
    }
    os << "}";
    return os;
}

int main() {
    static_assert(!IsContainer<int>::value, "expect false");
    static_assert(IsContainer<std::vector<int>>::value, "expect false");


#define SHOW(...) \
    { std::cout << #__VA_ARGS__ " = " << __VA_ARGS__ << '\n'; }

    std::vector<int> ivec{1, 3, 5, 7};
    std::vector<std::vector<int>> vec2d = {{1, 2, 4}, {10, 100, 1000}};
    std::cout << ivec << '\n';
    SHOW(ivec);
    SHOW(vec2d);
}

/**
output:

{ 1, 3, 5, 7, }
ivec = { 1, 3, 5, 7, }
vec2d = {
    { 1, 2, 4, },
    { 10, 100, 1000, },
}
 */
