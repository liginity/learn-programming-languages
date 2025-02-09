#ifndef LEARN_CPP_CXX11_SHARED_PTR_HPP
#define LEARN_CPP_CXX11_SHARED_PTR_HPP

#include <atomic>
#include <memory>
#include <type_traits>

namespace learn_cpp {

namespace detail {

template <class T, class D = std::default_delete<T>>
using unique_ptr = std::unique_ptr<T, D>;

template<class T>
class weak_ptr;

/*
   data member:
   - shared_owners_
   methods:
   - add_shared()
   - release_shared()
   - use_count()
   virtual methods:
   - on_zero_shared()
*/
class SharedCount {
   public:
    explicit SharedCount(long refs = 0) noexcept : shared_owners_(refs) {}

    virtual ~SharedCount();

    void add_shared() noexcept {
        shared_owners_.fetch_add(1, std::memory_order_relaxed);
    }

    bool release_shared() noexcept {
        if (shared_owners_.fetch_add(-1, std::memory_order_acq_rel) == -1) {
            on_zero_shared();
            return true;
        }
        return false;
    }

    long use_count() const noexcept {
        return shared_owners_.load(std::memory_order_relaxed) + 1;
    }

    virtual void on_zero_shared() noexcept = 0;

   private:
    std::atomic_long shared_owners_;
};

template<class T>
class shared_ptr
{
public:
#if __cplusplus < 201703L
    typedef T element_type; // until C++17
#else
    typedef std::remove_extent_t<T> element_type; // since C++17
    typedef weak_ptr<T> weak_type; // C++17
#endif

    // constructors:
    /*
    clang-format off

    constexpr shared_ptr() noexcept;
    template<class Y> explicit shared_ptr(Y* p);
    template<class Y, class D> shared_ptr(Y* p, D d);
    template<class Y, class D, class A> shared_ptr(Y* p, D d, A a);
    template <class D> shared_ptr(std::nullptr_t p, D d);
    template <class D, class A> shared_ptr(std::nullptr_t p, D d, A a);
    template<class Y> shared_ptr(const shared_ptr<Y>& r, T *p) noexcept;
    shared_ptr(const shared_ptr& r) noexcept;
    template<class Y> shared_ptr(const shared_ptr<Y>& r) noexcept;
    shared_ptr(shared_ptr&& r) noexcept;
    template<class Y> shared_ptr(shared_ptr<Y>&& r) noexcept;
    template<class Y> explicit shared_ptr(const weak_ptr<Y>& r);
    // template<class Y> shared_ptr(auto_ptr<Y>&& r);          // removed in C++17
    template <class Y, class D> shared_ptr(unique_ptr<Y, D>&& r);
    shared_ptr(std::nullptr_t) : shared_ptr() { }

    clang-format on
    */
    constexpr shared_ptr() noexcept = default;

    // destructor:
    ~shared_ptr();

    // assignment:
    /*

    clang-format off
    shared_ptr& operator=(const shared_ptr& r) noexcept;
    template<class Y> shared_ptr& operator=(const shared_ptr<Y>& r) noexcept;
    shared_ptr& operator=(shared_ptr&& r) noexcept;
    template<class Y> shared_ptr& operator=(shared_ptr<Y>&& r);
    // template<class Y> shared_ptr& operator=(auto_ptr<Y>&& r); // removed in C++17
    template <class Y, class D> shared_ptr& operator=(unique_ptr<Y, D>&& r);

    clang-format on
    */

    // modifiers:
    /*
    clang-format off

    void swap(shared_ptr& r) noexcept;
    void reset() noexcept;
    template<class Y> void reset(Y* p);
    template<class Y, class D> void reset(Y* p, D d);
    template<class Y, class D, class A> void reset(Y* p, D d, A a);

    clang-format on
    */

    // observers:
    /*
    clang-format off

    T* get() const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    long use_count() const noexcept;
    bool unique() const noexcept;
    explicit operator bool() const noexcept;
    template<class U> bool owner_before(shared_ptr<U> const& b) const noexcept;
    template<class U> bool owner_before(weak_ptr<U> const& b) const noexcept;

    clang-format on
    */

    private:
    element_type *ptr_;

};

template<class T>
class weak_ptr
{
public:
#if __cplusplus < 201703L
    typedef T element_type; // until C++17
#else
    typedef std::remove_extent_t<T> element_type; // since C++17
#endif

    // constructors
    constexpr weak_ptr() noexcept;
    template<class Y> weak_ptr(shared_ptr<Y> const& r) noexcept;
    weak_ptr(weak_ptr const& r) noexcept;
    template<class Y> weak_ptr(weak_ptr<Y> const& r) noexcept;
    weak_ptr(weak_ptr&& r) noexcept;                      // C++14
    template<class Y> weak_ptr(weak_ptr<Y>&& r) noexcept; // C++14

    // destructor
    ~weak_ptr();

    // assignment
    weak_ptr& operator=(weak_ptr const& r) noexcept;
    template<class Y> weak_ptr& operator=(weak_ptr<Y> const& r) noexcept;
    template<class Y> weak_ptr& operator=(shared_ptr<Y> const& r) noexcept;
    weak_ptr& operator=(weak_ptr&& r) noexcept;                      // C++14
    template<class Y> weak_ptr& operator=(weak_ptr<Y>&& r) noexcept; // C++14

    // modifiers
    void swap(weak_ptr& r) noexcept;
    void reset() noexcept;

    // observers
    long use_count() const noexcept;
    bool expired() const noexcept;
    shared_ptr<T> lock() const noexcept;
    template<class U> bool owner_before(shared_ptr<U> const& b) const noexcept;
    template<class U> bool owner_before(weak_ptr<U> const& b) const noexcept;
};

}
}

#endif
