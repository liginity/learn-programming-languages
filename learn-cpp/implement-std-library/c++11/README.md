Class template `std::vector`, from <https://en.cppreference.com/w/cpp/header/vector>.

``` cpp
namespace std {
  template<class T, class Allocator = std::allocator<T>>
  class vector
  {
  public:
    // types
    using value_type             = T;
    using allocator_type         = Allocator;
    using pointer                = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using size_type              = /* implementation-defined */;
    using difference_type        = /* implementation-defined */;
    using iterator               = /* implementation-defined */;
    using const_iterator         = /* implementation-defined */;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
 
    // construct/copy/destroy:
    vector();
    explicit vector(size_type n);
    vector(size_type n, const T& value,const Allocator& = Allocator());
    template <class InputIterator>
        vector(InputIterator first, InputIterator last,const Allocator& = Allocator());
    vector(const vector<T,Allocator>& x);
    vector(vector&&);
    vector(const vector&, const Allocator&);
    vector(vector&&, const Allocator&);
    vector(std::initializer_list<T>, const Allocator& = Allocator());
 
    ~vector();
    vector<T,Allocator>& operator=(const vector<T,Allocator>& x);
    vector<T,Allocator>& operator=(vector<T,Allocator>&& x);
    vector& operator=(std::initializer_list<T>);
    template <class InputIterator>
        void assign(InputIterator first, InputIterator last);
    void assign(size_type n, const T& t);
    void assign(std::initializer_list<T>);
    allocator_type get_allocator() const noexcept;
 
    // iterators:
    iterator                begin() noexcept;
    const_iterator          begin() const noexcept;
    iterator                end() noexcept;
    const_iterator          end() const noexcept;
 
    reverse_iterator        rbegin() noexcept;
    const_reverse_iterator  rbegin() const noexcept;
    reverse_iterator        rend() noexcept;
    const_reverse_iterator  rend() const noexcept;
 
    const_iterator          cbegin() const noexcept;
    const_iterator          cend() const noexcept;
    const_reverse_iterator  crbegin() const noexcept;
    const_reverse_iterator  crend() const noexcept;
 
    // capacity:
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void      resize(size_type sz);
    void      resize(size_type sz, const T& c);
    size_type capacity() const noexcept;
    bool      empty() const noexcept;
    void      reserve(size_type n);
    void      shrink_to_fit();
 
    // element access:
    reference       operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference       at(size_type n);
    const_reference at(size_type n) const;
    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;
 
    //data access
    T*       data() noexcept;
    const T* data() const noexcept;
 
    // modifiers:
    template <class... Args> void emplace_back(Args&&... args);
    void push_back(const T& x);
    void push_back(T&& x);
    void pop_back();
 
    template <class... Args> iterator emplace(const_iterator position, Args&&... args);
    iterator insert(const_iterator position, const T& x);
    iterator insert(const_iterator position, T&& x);
    iterator insert(const_iterator position, size_type n, const T& x);
    template <class InputIterator>
        iterator insert (const_iterator position, InputIterator first, 
                         InputIterator last);
    iterator insert(const_iterator position, std::initializer_list<T>);
 
    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    void     swap(vector<T,Allocator>&);
    void     clear() noexcept;
};

}
```

> NOTE This class template definition is combined from several versions on the https://en.cppreference.com/w/cpp/header/vector page.
>
> I hope it complies with C++11.



Class template `std::shared_ptr`, from llvm-14 (`/usr/lib/llvm-14/include/c++/v1/memory` on debian).

``` cpp
template<class T>
class shared_ptr
{
public:
    typedef T element_type; // until C++17
    typedef remove_extent_t<T> element_type; // since C++17
    typedef weak_ptr<T> weak_type; // C++17

    // constructors:
    constexpr shared_ptr() noexcept;
    template<class Y> explicit shared_ptr(Y* p);
    template<class Y, class D> shared_ptr(Y* p, D d);
    template<class Y, class D, class A> shared_ptr(Y* p, D d, A a);
    template <class D> shared_ptr(nullptr_t p, D d);
    template <class D, class A> shared_ptr(nullptr_t p, D d, A a);
    template<class Y> shared_ptr(const shared_ptr<Y>& r, T *p) noexcept;
    shared_ptr(const shared_ptr& r) noexcept;
    template<class Y> shared_ptr(const shared_ptr<Y>& r) noexcept;
    shared_ptr(shared_ptr&& r) noexcept;
    template<class Y> shared_ptr(shared_ptr<Y>&& r) noexcept;
    template<class Y> explicit shared_ptr(const weak_ptr<Y>& r);
    template<class Y> shared_ptr(auto_ptr<Y>&& r);          // removed in C++17
    template <class Y, class D> shared_ptr(unique_ptr<Y, D>&& r);
    shared_ptr(nullptr_t) : shared_ptr() { }

    // destructor:
    ~shared_ptr();

    // assignment:
    shared_ptr& operator=(const shared_ptr& r) noexcept;
    template<class Y> shared_ptr& operator=(const shared_ptr<Y>& r) noexcept;
    shared_ptr& operator=(shared_ptr&& r) noexcept;
    template<class Y> shared_ptr& operator=(shared_ptr<Y>&& r);
    template<class Y> shared_ptr& operator=(auto_ptr<Y>&& r); // removed in C++17
    template <class Y, class D> shared_ptr& operator=(unique_ptr<Y, D>&& r);

    // modifiers:
    void swap(shared_ptr& r) noexcept;
    void reset() noexcept;
    template<class Y> void reset(Y* p);
    template<class Y, class D> void reset(Y* p, D d);
    template<class Y, class D, class A> void reset(Y* p, D d, A a);

    // observers:
    T* get() const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    long use_count() const noexcept;
    bool unique() const noexcept;
    explicit operator bool() const noexcept;
    template<class U> bool owner_before(shared_ptr<U> const& b) const noexcept;
    template<class U> bool owner_before(weak_ptr<U> const& b) const noexcept;
};
```
