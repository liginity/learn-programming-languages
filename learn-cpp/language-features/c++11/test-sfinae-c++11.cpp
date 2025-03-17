#include <type_traits>
#include <vector>

class A {
    // NOTE 我之前漏掉了 public！当时感觉见鬼了！！！
public:
    typedef const int* const_iterator;
    typedef int* iterator;
};

class B {
public:
    typedef int* iterator;
};

/*
// not working
template <typename T, typename = void>
struct has_const_iterator : std::false_type {};

template <typename T, typename = decltype(std::declval<typename T::const_iterator>())>
struct has_const_iterator : std::true_type {};
 */


// // this implementation is not correct!!!
// template <typename... Ts>
// struct void_t {
//     typedef void type;
// };

// Custom void_t implementation for C++11
template <typename...>
using void_t = void;


template <typename T, typename = void>
struct has_const_iterator : std::false_type {};

// // not working!
// template <typename T>
// struct has_const_iterator<T, decltype(std::declval<typename T::const_iterator>())> : std::true_type {};

// // not working!
// template <typename T>
// struct has_const_iterator<T, typename T::const_iterator> : std::true_type {};

template <typename T>
struct has_const_iterator<T, void_t<typename T::const_iterator>> : std::true_type {};


// // Primary template (fallback case: false)
// template <typename, typename = void>
// struct has_const_iterator : std::false_type {};

// // Specialization for types that have a member type `const_iterator`
// template <typename T>
// struct has_const_iterator<T, void_t<typename T::const_iterator>> : std::true_type {};


// template <typename T, typename = void_t<typename T::const_iterator>>
// struct has_const_iterator : std::true_type {};

// template <typename T>
// struct has_const_iterator<T> : std::false_type {};

int main() {
    static_assert(has_const_iterator<std::vector<int>>::value, "std::vector<int>::const_iterator");
    static_assert(has_const_iterator<A>::value, "A has const_iterator");
}
