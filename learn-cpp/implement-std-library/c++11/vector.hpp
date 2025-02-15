#ifndef LEARN_CPP_CXX11_VECTOR_HPP
#define LEARN_CPP_CXX11_VECTOR_HPP

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <type_traits>

namespace learn_cpp {

namespace detail {

inline namespace v1 {
// NOTE v1::vector is relatively simple.

#if defined(DEBUG_VECTOR)
#define ASSERT(expr, text) assert(expr)
#else
#define ASSERT(expr, text)
#endif

template <class T, class Allocator = std::allocator<T>>
class vector {
   public:
    // types
    // clang-format off
    using value_type             = T;
    using allocator_type         = Allocator;
    using pointer                = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using size_type              = std::size_t;                        /* implementation-defined */
    using difference_type        = std::make_signed<size_type>::type;  /* implementation-defined */
    using iterator               = T*;                                 /* implementation-defined */
    using const_iterator         = const T*;                           /* implementation-defined */
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    // clang-format on

    // construct/copy/destroy:
    vector() = default;

    explicit vector(size_type n) {
        allocate_mem_(n);
        construct_n_at_end_(n);
    }

    vector(size_type n, const T& value, const Allocator& a = Allocator())
        : alloc_(a) {
        allocate_mem_(n);
        construct_n_at_end_(n, value);
    }
    template <class InputIterator>
    vector(InputIterator first, InputIterator last,
           const Allocator& = Allocator());

    vector(const vector<T, Allocator>& x) : alloc_(x.alloc_) {
        size_type n = x.size();
        allocate_mem_(n);
        std::uninitialized_copy(x.begin_, x.end_, begin_);
        end_ = begin_ + n;
        end_cap_ = begin_ + n;
    }

    vector(vector&& x)
        : begin_(x.begin_),
          end_(x.end_),
          end_cap_(x.end_cap_),
          alloc_(std::move(x.alloc_)) {
        x.begin_ = x.end_ = nullptr;
        x.end_cap_ = nullptr;
    }

    vector(const vector&, const Allocator&);
    vector(vector&&, const Allocator&);

    vector(std::initializer_list<T> ilist, const Allocator& a = Allocator())
        : alloc_(a) {
        size_type n = ilist.size();
        allocate_mem_(n);
        std::uninitialized_copy_n(ilist.begin(), n, begin_);
        end_ = begin_ + n;
        end_cap_ = begin_ + n;
    }

    ~vector() {
        if (begin_ != nullptr) {
            clear_();
            deallocate_mem_();
        }
    }

    vector<T, Allocator>& operator=(const vector<T, Allocator>& x) {
        // NOTE the allocator is not propagated.
        if (this == std::addressof(x)) {
            return *this;
        }
        auto n = x.size();
        assign_n_(x.begin_, n);
    }

    vector<T, Allocator>& operator=(vector<T, Allocator>&& x) {
        // NOTE the allocator is not propagated.
        if (this == std::addressof(x)) {
            return *this;
        }
        if (begin_ != nullptr) {
            clear_();
            deallocate_mem_();
        }
        begin_ = x.begin_;
        end_ = x.end_;
        end_cap_ = x.end_cap_;
        alloc_ = std::move(x.alloc_);
        x.begin_ = x.end_ = nullptr;
        x.end_cap_ = nullptr;
    }

    vector& operator=(std::initializer_list<T> ilist) {
        // NOTE the allocator is not propagated.
        auto n = ilist.size();
        assign_n_(ilist.begin(), n);
    }
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);
    void assign(size_type n, const T& t);
    void assign(std::initializer_list<T>);

    allocator_type get_allocator() const noexcept { return alloc_(); }

    // iterators:
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;

    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // capacity:
    size_type size() const noexcept { return end_ - begin_; }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    void resize(size_type sz);
    void resize(size_type sz, const T& c);

    size_type capacity() const noexcept { return end_cap_ - begin_; }

    bool empty() const noexcept { return end_ == begin_; }

    void reserve(size_type n);
    void shrink_to_fit();

    // element access:
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    // data access
    T* data() noexcept { return begin_; }

    const T* data() const noexcept { return begin_; }

    // modifiers:
    template <class... Args>
    void emplace_back(Args&&... args);
    void push_back(const T& x);
    void push_back(T&& x);
    void pop_back();

    template <class... Args>
    iterator emplace(const_iterator position, Args&&... args);
    iterator insert(const_iterator position, const T& x);
    iterator insert(const_iterator position, T&& x);
    iterator insert(const_iterator position, size_type n, const T& x);
    template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first,
                    InputIterator last);
    iterator insert(const_iterator position, std::initializer_list<T>);

    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    void swap(vector<T, Allocator>&);
    void clear() noexcept;

   private:
    pointer begin_ = nullptr;
    pointer end_ = nullptr;
    // end of capacity. capacity() == end_cap_ ? (end_cap_ - end_) : 0;
    pointer end_cap_ = nullptr;
    allocator_type alloc_;

    pointer get_end_cap_() noexcept { return end_cap_; }

    const_pointer get_end_cap_() const noexcept { return end_cap_; }

    allocator_type& get_alloc_() noexcept { return alloc_; }

    /** Allocate memory for n value_type.
        This is used mostly at construction.
     */
    void allocate_mem_(size_type n);
    /** Deallocate memory. Set data members to nullptr.
     */
    void deallocate_mem_();
    void ensure_capacity_(size_type n);
    size_type suggest_capacity_(size_type at_least_cap);

    template <class... Args>
    void construct_n_at_end_(size_type n, Args&&... args);
    // void construct_n_at_end_(size_type n);
    template <class... Args>
    void construct_one_at_end_(Args&&... args);
    // void construct_one_at_end_(const value_type& x);
    void clear_() noexcept;
    template <typename InputIterator>
    void assign_n_(InputIterator first, size_type n);
};

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() noexcept {
    return begin_;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin()
    const noexcept {
    return begin_;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept {
    return end_;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end()
    const noexcept {
    return end_;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin()
    const noexcept {
    return begin_;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend()
    const noexcept {
    return end_;
}

template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type sz) {
    auto old_size = size();
    if (sz == old_size) {
        return;
    }
    std::allocator_traits<allocator_type> alloc_trait;
    if (sz > old_size) {
        ensure_capacity_(sz);
        for (auto i = old_size; i < sz; ++i) {
            alloc_trait.construct(get_alloc_(), end_);
            ++end_;
        }
        return;
    }
    // sz < old_size
    for (auto i = sz; i < old_size; ++i) {
        alloc_trait.destroy(get_alloc_(), --end_);
    }
}

template <class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](
    size_type n) {
    ASSERT(n < size(), "out of range access");
    return begin_[n];
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](
    size_type n) const {
    ASSERT(n < size(), "out of range access");
    return begin_[n];
}

template <class T, class Allocator>
template <class... Args>
void vector<T, Allocator>::emplace_back(Args&&... args) {
    ensure_capacity_(size() + 1);
    construct_one_at_end_(std::forward<Args>(args)...);
}

template <class T, class Allocator>
void vector<T, Allocator>::push_back(const T& x) {
    ensure_capacity_(size() + 1);
    construct_one_at_end_(x);
}

template <class T, class Allocator>
void vector<T, Allocator>::push_back(T&& x) {
    ensure_capacity_(size() + 1);
    construct_one_at_end_(x);
}

template <class T, class Allocator>
void vector<T, Allocator>::pop_back() {
    std::allocator_traits<allocator_type> alloc_trait;
    alloc_trait.destroy(get_alloc_(), end_);
    --end_;
}

template <class T, class Allocator>
void vector<T, Allocator>::swap(vector& x) {
    using std::swap;
    swap(begin_, x.begin_);
    swap(end_, x.end_);
    swap(end_cap_, x.end_cap_);
    swap(alloc_, x.alloc_);
}

template <class T, class Allocator>
void vector<T, Allocator>::clear() noexcept {
    clear_();
}

// private methods

template <class T, class Allocator>
void vector<T, Allocator>::allocate_mem_(size_type n) {
    ASSERT(begin_ == nullptr && end_ == nullptr,
           "should call this for vector that has no allocation");

    begin_ = end_ =
        std::allocator_traits<allocator_type>{}.allocate(get_alloc_(), n);
    end_cap_ = begin_ + n;
}

template <class T, class Allocator>
void vector<T, Allocator>::deallocate_mem_() {
    std::allocator_traits<allocator_type>::deallocate(get_alloc_(), begin_,
                                                      capacity());
    begin_ = end_ = nullptr;
    end_cap_ = nullptr;
}

template <class T, class Allocator>
void vector<T, Allocator>::ensure_capacity_(size_type n) {
    if (!(capacity() < n)) {
        return;
    }
    auto old_size = size();
    auto new_capacity = suggest_capacity_(n);
    std::allocator_traits<allocator_type> alloc_trait;
    pointer new_begin_ = alloc_trait.allocate(get_alloc_(), new_capacity);
    if (begin_ != nullptr) {
        // NOTE Could use move here
        std::uninitialized_copy_n(begin_, old_size, new_begin_);
        clear_();
        deallocate_mem_();
    }
    begin_ = new_begin_;
    end_ = begin_ + old_size;
    end_cap_ = begin_ + new_capacity;
}

template <class T, class Allocator>
typename vector<T, Allocator>::size_type
vector<T, Allocator>::suggest_capacity_(size_type at_least_cap) {
    //
    auto n = capacity();
    if (at_least_cap < n) {
        return n;
    }
    return std::max(2 * n, at_least_cap);
}

// assume capacity is enough.
template <class T, class Allocator>
template <class... Args>
void vector<T, Allocator>::construct_n_at_end_(size_type n, Args&&... args) {
    std::allocator_traits<allocator_type> alloc_trait;
    for (int i = 0; i < n; ++i) {
        alloc_trait.construct(get_alloc_(), end_, std::forward<Args>(args)...);
        ++end_;
    }
}

// assume capacity is enough.
template <class T, class Allocator>
template <class... Args>
void vector<T, Allocator>::construct_one_at_end_(Args&&... args) {
    std::allocator_traits<allocator_type> alloc_trait;
    alloc_trait.construct(get_alloc_(), end_, std::forward<Args>(args)...);
    ++end_;
}

template <class T, class Allocator>
void vector<T, Allocator>::clear_() noexcept {
    if (begin_ == nullptr) {
        return;
    }
    std::allocator_traits<allocator_type> alloc_trait;
    for (auto p = begin_; p != end_; ++p) {
        alloc_trait.destroy(get_alloc_(), p);
    }
}

template <class T, class Allocator>
template <typename InputIterator>
void vector<T, Allocator>::assign_n_(InputIterator first, size_type n) {
    clear_();
    ensure_capacity_(n);
    for (size_type i = 0; i < n; ++i) {
        std::allocator_traits<allocator_type>::consturct(get_alloc_(), end_,
                                                         *first);
        ++end_;
        ++first;
    }
}

// Add an alias Vector for vector.
template <class T, class Allocator = std::allocator<T>>
using Vector = vector<T, Allocator>;

#undef ASSERT

}  // namespace v1
}  // namespace detail
}  // namespace learn_cpp
#endif
