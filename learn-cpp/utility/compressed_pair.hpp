#ifndef COMPRESSED_PAIR_H
#define COMPRESSED_PAIR_H

#include <type_traits>

namespace learn_cpp {
namespace detail {

template <typename T, bool CanBeEmptyBase =
                          std::is_empty<T>::value && !std::is_final<T>::value>
class CompressedPairItem {
   public:
    typedef T ParamT;
    typedef T& reference;
    typedef const T& const_reference;

    CompressedPairItem() = default;

    reference get() noexcept { return value_; }

    const_reference get() const noexcept { return value_; }

   private:
    T value_;
};

template <typename T>
class CompressedPairItem<T, true> : private T {
   public:
    typedef T ParamT;
    typedef T& reference;
    typedef const T& const_reference;

    CompressedPairItem() = default;

    reference get() noexcept { return *this; }

    const_reference get() const noexcept { return *this; }
};

template <typename T1, typename T2>
class CompressedPair : CompressedPairItem<T1>, CompressedPairItem<T2> {
   public:
    typedef CompressedPairItem<T1> Base1;
    typedef CompressedPairItem<T2> Base2;

    typename Base1::reference first() noexcept {
        return static_cast<Base1&>(*this).get();
    }

    typename Base1::const_reference first() const noexcept {
        return static_cast<Base1 const&>(*this).get();
    }

    typename Base2::reference second() noexcept {
        return static_cast<Base2&>(*this).get();
    }

    typename Base2::const_reference second() const noexcept {
        return static_cast<Base2 const&>(*this).get();
    }

    void swap(CompressedPair& x) {
        using std::swap;
        swap(first(), x.first());
        swap(second(), x.second());
    }
};

}  // namespace detail
}  // namespace learn_cpp

#endif
