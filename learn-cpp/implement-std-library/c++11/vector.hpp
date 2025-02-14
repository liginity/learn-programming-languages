#ifndef LEARN_CPP_CXX11_VECTOR_HPP
#define LEARN_CPP_CXX11_VECTOR_HPP

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <type_traits>

namespace learn_cpp {

namespace detail {

inline namespace v1 {
// NOTE v1::vector is relatively simple.

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
        allocate_n_at_end_(n);
        construct_n_at_end_(n);
    }

    vector(size_type n, const T& value, const Allocator& a = Allocator())
        : alloc_(a) {
        allocate_n_at_end_(n);
        construct_n_at_end_(n, value);
    }
    template <class InputIterator>
    vector(InputIterator first, InputIterator last,
           const Allocator& = Allocator());

    vector(const vector<T, Allocator>& x) : alloc_(x.alloc_) {
        size_type n = x.size();
        allocate_n_at_end_(n);
        std::uninitialized_copy(x.begin_, x.end_, begin_);
        end_ = begin_ + n;
    }

    vector(vector&& x)
        : begin_(x.begin_),
          end_(x.end_),
          end_cap_(x.end_cap_),
          alloc_(std::move(x.alloc_)) {}

    vector(const vector&, const Allocator&);
    vector(vector&&, const Allocator&);

    vector(std::initializer_list<T> ilist, const Allocator& a = Allocator())
        : alloc_(a) {
        size_type n = ilist.size();
        allocate_n_at_end_(n);
        std::uninitialized_copy_n(ilist.begin(), n, begin_);
        end_ = begin_ + n;
    }

    ~vector() {}

    vector<T, Allocator>& operator=(const vector<T, Allocator>& x) {
        // NOTE the allocator is not propagated.
        if (this == std::addressof(x)) {
            return *this;
        }
        clear_();
        vector temp(x);
        swap(temp);
    }

    vector<T, Allocator>& operator=(vector<T, Allocator>&& x) {
        // NOTE the allocator is not propagated.
        if (this == std::addressof(x)) {
            return *this;
        }
        clear_();
        vector temp(std::move(x));
        swap(temp);
    }

    vector& operator=(std::initializer_list<T> ilist) {
        // NOTE the allocator is not propagated.
        clear_();
        vector temp(ilist);
        swap(temp);
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
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void resize(size_type sz);
    void resize(size_type sz, const T& c);
    size_type capacity() const noexcept;
    bool empty() const noexcept;
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
    T* data() noexcept;
    const T* data() const noexcept;

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

    allocator_type get_alloc_() noexcept { return alloc_; }

    void allocate_n_at_end_(size_type n);

    void construct_n_at_end_(size_type n);
    void clear_();
};

// Add an alias Vector for vector.
template <class T, class Allocator = std::allocator<T>>
using Vector = vector<T, Allocator>;

}  // namespace v1
}  // namespace detail
}  // namespace learn_cpp
#endif
