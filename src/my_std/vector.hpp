#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>
#include <new>
#include <cassert>
#include <memory>

namespace my
{
    template <typename T>
    class vector_iterator
    {
    public:
        using iterator_concept = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using element_type = value_type;

        vector_iterator() = default;
        explicit vector_iterator(T* ptr);

        bool operator== (const vector_iterator<T>& other) const;
        std::strong_ordering operator<=> (const vector_iterator<T>& other) const;

        vector_iterator& operator++ ();
        vector_iterator operator++ (int);

        vector_iterator& operator-- ();
        vector_iterator operator-- (int);

        vector_iterator& operator+= (std::ptrdiff_t n);
        template<typename U>
        friend vector_iterator<U> operator+ (vector_iterator<U> i, std::ptrdiff_t n);
        template<typename U>
        friend vector_iterator<U> operator+ (std::ptrdiff_t n, vector_iterator<U> i);

        vector_iterator& operator-= (std::ptrdiff_t n);
        template<typename U>
        friend std::ptrdiff_t operator- (const vector_iterator<U>& a, const vector_iterator<U>& b);

        T& operator[] (std::ptrdiff_t n) const;
        T& operator* () const;
        T* operator-> () const;
    private:
        T* ptr{};
    };

    template <typename T>
    vector_iterator<T>::vector_iterator(T* ptr) : ptr(ptr)
    {

    }

    template <typename T>
    bool vector_iterator<T>::operator== (const vector_iterator<T>& other) const
    {
        return ptr == other.ptr;
    }

    template<typename T>
    std::strong_ordering vector_iterator<T>::operator<=> (const vector_iterator<T>& other) const
    {
        return ptr <=> other.ptr;
    }

    template <typename T>
    vector_iterator<T>& vector_iterator<T>::operator++ ()
    {
        ++ptr;
        return *this;
    }

    template <typename T>
    vector_iterator<T> vector_iterator<T>::operator++ (int)
    {
        const auto copy = *this;
        ++ptr;
        return copy;
    }

    template <typename T>
    vector_iterator<T>& vector_iterator<T>::operator-- ()
    {
        --ptr;
        return *this;
    }

    template <typename T>
    vector_iterator<T> vector_iterator<T>::operator-- (int)
    {
        const auto copy = *this;
        --ptr;
        return copy;
    }

    template<typename T>
    vector_iterator<T>& vector_iterator<T>::operator+= (std::ptrdiff_t n)
    {
        ptr += n;
        return *this;
    }

    template <typename T>
    vector_iterator<T> operator+ (vector_iterator<T> i, std::ptrdiff_t n)
    {
        i += n;
        return i;
    }

    template <typename T>
    vector_iterator<T> operator+ (std::ptrdiff_t n, vector_iterator<T> i)
    {
        i += n;
        return i;
    }

    template<typename T>
    vector_iterator<T>& vector_iterator<T>::operator-= (std::ptrdiff_t n)
    {
        ptr -= n;
        return *this;
    }

    template <typename T>
    vector_iterator<T> operator- (vector_iterator<T> i, std::ptrdiff_t n)
    {
        i -= n;
        return i;
    }

    template <typename T>
    std::ptrdiff_t operator- (const vector_iterator<T>& a, const vector_iterator<T>& b)
    {
        return a.ptr - b.ptr;
    }

    template<typename T>
    T& vector_iterator<T>::operator[] (std::ptrdiff_t n) const
    {
        return *(ptr + n);
    }

    template <typename T>
    T& vector_iterator<T>::operator* () const
    {
        return *ptr;
    }

    template <typename T>
    T* vector_iterator<T>::operator-> () const
    {
        return ptr;
    }


    template <typename T>
    class vector_const_iterator
    {
    public:
        using iterator_concept = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        using element_type = const value_type;

        vector_const_iterator() = default;
        explicit vector_const_iterator(T* ptr);

        bool operator== (const vector_const_iterator<T>& other) const;
        std::strong_ordering operator<=> (const vector_const_iterator<T>& other) const;

        vector_const_iterator& operator++ ();
        vector_const_iterator operator++ (int);

        vector_const_iterator& operator-- ();
        vector_const_iterator operator-- (int);

        vector_const_iterator& operator+= (std::ptrdiff_t n);
        template<typename U>
        friend vector_const_iterator<U> operator+ (vector_const_iterator<U> i, std::ptrdiff_t n);
        template<typename U>
        friend vector_const_iterator<U> operator+ (std::ptrdiff_t n, vector_const_iterator<U> i);

        vector_const_iterator& operator-= (std::ptrdiff_t n);
        template<typename U>
        friend std::ptrdiff_t operator- (const vector_const_iterator<U>& a, const vector_const_iterator<U>& b);

        reference operator[] (std::ptrdiff_t n) const;
        reference operator* () const;
        pointer operator-> () const;
    private:
        T* ptr{};
    };

    template <typename T>
    vector_const_iterator<T>::vector_const_iterator(T* ptr) : ptr(ptr)
    {

    }

    template <typename T>
    bool vector_const_iterator<T>::operator== (const vector_const_iterator<T>& other) const
    {
        return ptr == other.ptr;
    }

    template<typename T>
    std::strong_ordering vector_const_iterator<T>::operator<=> (const vector_const_iterator<T>& other) const
    {
        return ptr <=> other.ptr;
    }

    template <typename T>
    vector_const_iterator<T>& vector_const_iterator<T>::operator++ ()
    {
        ++ptr;
        return *this;
    }

    template <typename T>
    vector_const_iterator<T> vector_const_iterator<T>::operator++ (int)
    {
        const auto copy = *this;
        ++ptr;
        return copy;
    }

    template <typename T>
    vector_const_iterator<T>& vector_const_iterator<T>::operator-- ()
    {
        --ptr;
        return *this;
    }

    template <typename T>
    vector_const_iterator<T> vector_const_iterator<T>::operator-- (int)
    {
        const auto copy = *this;
        --ptr;
        return copy;
    }

    template<typename T>
    vector_const_iterator<T>& vector_const_iterator<T>::operator+= (std::ptrdiff_t n)
    {
        ptr += n;
        return *this;
    }

    template <typename T>
    vector_const_iterator<T> operator+ (vector_const_iterator<T> i, std::ptrdiff_t n)
    {
        i += n;
        return i;
    }

    template <typename T>
    vector_const_iterator<T> operator+ (std::ptrdiff_t n, vector_const_iterator<T> i)
    {
        i += n;
        return i;
    }

    template<typename T>
    vector_const_iterator<T>& vector_const_iterator<T>::operator-= (std::ptrdiff_t n)
    {
        ptr -= n;
        return *this;
    }

    template <typename T>
    vector_const_iterator<T> operator- (vector_const_iterator<T> i, std::ptrdiff_t n)
    {
        i -= n;
        return i;
    }

    template <typename T>
    std::ptrdiff_t operator- (const vector_const_iterator<T>& a, const vector_const_iterator<T>& b)
    {
        return a.ptr - b.ptr;
    }

    template<typename T>
    vector_const_iterator<T>::reference vector_const_iterator<T>::operator[] (std::ptrdiff_t n) const
    {
        return *(ptr + n);
    }

    template <typename T>
    vector_const_iterator<T>::reference vector_const_iterator<T>::operator* () const
    {
        return *ptr;
    }

    template <typename T>
    vector_const_iterator<T>::pointer vector_const_iterator<T>::operator-> () const
    {
        return ptr;
    }


    template <typename I>
    class reverse_iterator
    {
    public:
        using iterator_type = I;
        using iterator_concept = std::random_access_iterator_tag; // If Iter models std::random_access_iterator, this is std::random_access_iterator_tag. Otherwise, this is std::bidirectional_iterator_tag
        using iterator_category = std::iterator_traits<I>::iterator_category; //If std::iterator_traits<Iter>::iterator_category models std::derived_from<std::random_access_iterator_tag>, this is std::random_access_iterator_tag. Otherwise, this is std::iterator_traits<Iter>::iterator_category
        using value_type = std::iter_value_t<I>;
        using difference_type = std::iter_difference_t<I>;
        using pointer = std::iterator_traits<I>::pointer;
        using reference = std::iter_reference_t<I>;

        reverse_iterator() = default;
        explicit reverse_iterator(I iter);

        bool operator== (const reverse_iterator& other) const;
        std::strong_ordering operator<=> (const reverse_iterator& other) const;

        reverse_iterator& operator++ ();
        reverse_iterator operator++ (int);

        reverse_iterator& operator-- ();
        reverse_iterator operator-- (int);

        reverse_iterator& operator+= (std::ptrdiff_t n);
        template <typename U>
        friend reverse_iterator<U> operator+ (reverse_iterator<U> i, std::ptrdiff_t n);
        template <typename U>
        friend reverse_iterator<U> operator+ (std::ptrdiff_t n, reverse_iterator<U> i);

        reverse_iterator& operator-= (std::ptrdiff_t n);
        template <typename U>
        friend std::ptrdiff_t operator- (const reverse_iterator<U>& a, const reverse_iterator<U>& b);

        reference operator[] (std::ptrdiff_t n) const;
        reference operator* () const;
        pointer operator-> () const;
    private:
        I iter{};
    };

    template <typename I>
    reverse_iterator<I>::reverse_iterator(I iter) : iter(iter)
    {

    }

    template <typename I>
    bool reverse_iterator<I>::operator== (const reverse_iterator<I>& other) const
    {
        return iter == other.iter;
    }

    template<typename I>
    std::strong_ordering reverse_iterator<I>::operator<=> (const reverse_iterator<I>& other) const
    {
        return other.iter <=> iter;
    }

    template <typename I>
    reverse_iterator<I>& reverse_iterator<I>::operator++ ()
    {
        --iter;
        return *this;
    }

    template <typename I>
    reverse_iterator<I> reverse_iterator<I>::operator++ (int)
    {
        const auto copy = *this;
        --iter;
        return copy;
    }

    template <typename I>
    reverse_iterator<I>& reverse_iterator<I>::operator-- ()
    {
        ++iter;
        return *this;
    }

    template <typename I>
    reverse_iterator<I> reverse_iterator<I>::operator-- (int)
    {
        const auto copy = *this;
        ++iter;
        return copy;
    }

    template<typename I>
    reverse_iterator<I>& reverse_iterator<I>::operator+= (std::ptrdiff_t n)
    {
        iter -= n;
        return *this;
    }

    template <typename I>
    reverse_iterator<I> operator+ (reverse_iterator<I> i, std::ptrdiff_t n)
    {
        i += n; // logic is already handled by implementation of +=
        return i;
    }

    template <typename I>
    reverse_iterator<I> operator+ (std::ptrdiff_t n, reverse_iterator<I> i)
    {
        i += n;
        return i;
    }

    template<typename I>
    reverse_iterator<I>& reverse_iterator<I>::operator-= (std::ptrdiff_t n)
    {
        iter += n;
        return *this;
    }

    template <typename I>
    reverse_iterator<I> operator- (reverse_iterator<I> i, std::ptrdiff_t n)
    {
        i -= n; // logic is already handled by implementation of +=
        return i;
    }

    template <typename I>
    std::ptrdiff_t operator- (const reverse_iterator<I>& a, const reverse_iterator<I>& b)
    {
        return b.iter - a.iter;
    }

    template<typename I>
    reverse_iterator<I>::reference reverse_iterator<I>::operator[] (std::ptrdiff_t n) const
    {
        auto i = std::prev(iter);
        std::advance(i, -n);
        return *i;
    }

    template <typename I>
    reverse_iterator<I>::reference reverse_iterator<I>::operator* () const
    {
        return *std::prev(iter);
    }

    template <typename I>
    reverse_iterator<I>::pointer reverse_iterator<I>::operator-> () const
    {
        return operator->(std::prev(iter));
    }


    template <typename T, typename A = std::allocator<T>>
    class vector
    {
    public:
        // Member types
        using value_type = T;

        using reference = T&;
        using const_reference = const T&;

        using pointer = T*;
        using const_pointer = const T*;
        
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using iterator = vector_iterator<T>;
        using const_iterator = vector_const_iterator<T>;

        using reverse_iterator = my::reverse_iterator<iterator>; // Here we shadow reverse_iterator class template, be careful
        using const_reverse_iterator = my::reverse_iterator<const_iterator>;

        // Constructors
        constexpr vector() noexcept(noexcept(A()));
        constexpr explicit vector(const A& allocator) noexcept;
        constexpr vector(size_type size, const A& allocator = A());
        constexpr explicit vector(size_type size, const T& value, const A& allocator = A());
        template <std::input_iterator I>
        constexpr vector(I begin, I end, const A& allocator = A());
        constexpr vector(std::initializer_list<T> init_list, const A& allocator = A());

        // Rule of 5
        constexpr vector(const vector& other);
        constexpr vector& operator= (vector other);
        constexpr vector(vector&& other) noexcept;
        constexpr vector& operator= (vector&& other);
        ~vector();

        constexpr void swap(vector& other) noexcept;

        // Propery access
        constexpr bool empty() const noexcept;
        constexpr size_type size() const noexcept;
        constexpr size_type max_size() const noexcept;
        constexpr size_type capacity() const noexcept;
        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;

        // Comparison
        constexpr bool operator== (const vector& other) const;
        constexpr auto operator<=> (const vector& other) const;

        // Element access
        constexpr reference operator[] (size_type index);
        constexpr const_reference operator[] (size_type index) const;
        constexpr reference at(size_type index);
        constexpr const_reference at(size_type index) const;
        constexpr reference front();
        constexpr const_reference front() const;
        constexpr reference back();
        constexpr const_reference back() const;

        // Adding/removing elements
        constexpr void push_back(const T& value);
        constexpr void push_back(T&& value);
        constexpr void pop_back(); // not noexcept because reasons https://stackoverflow.com/questions/55222295/when-can-stdvectorpop-back-throw-an-exception
        template <typename ... Args>
        constexpr reference emplace_back(Args&& ... args);

        // Size/capacity modification
        constexpr void clear() noexcept;
        constexpr void resize(size_type new_size);
        constexpr void resize(size_type new_size, const T& value);
        constexpr void reserve(size_type new_capacity);
        constexpr void shrink_to_fit();

        // Iterators
        constexpr iterator begin() noexcept;
        constexpr iterator end() noexcept;
        constexpr const_iterator begin() const noexcept;
        constexpr const_iterator end() const noexcept;
        constexpr const_iterator cbegin() const noexcept;
        constexpr const_iterator cend() const noexcept;

        // Reverse iterators
        constexpr reverse_iterator rbegin() noexcept;
        constexpr reverse_iterator rend() noexcept;
        constexpr const_reverse_iterator rbegin() const noexcept;
        constexpr const_reverse_iterator rend() const noexcept;
        constexpr const_reverse_iterator crbegin() const noexcept;
        constexpr const_reverse_iterator crend() const noexcept;

    private:
        bool is_memory_filled() const;
        void grow();

        A allocator_ { };
        size_type size_ {0};
        size_type capacity_ {0};
        T* data_ {nullptr};
    };

    // Constructors
    template <typename T, typename A>
    constexpr vector<T, A>::vector() noexcept(noexcept(A()))
    {

    }

    // Constructors
    template <typename T, typename A>
    constexpr vector<T, A>::vector(const A& allocator) noexcept : allocator_{allocator}
    {

    }

    template <typename T, typename A>
    constexpr vector<T, A>::vector(size_type size, const A& allocator) :
        allocator_{allocator},
        size_{size},
        capacity_{size},
        data_{std::allocator_traits<A>::allocate(allocator_, size)}
    {
        T* begin = data_;
        T* end = data_ + size_;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            std::allocator_traits<A>::construct(allocator_, ptr);
        }
    }

    template <typename T, typename A>
    constexpr vector<T, A>::vector(size_type size, const T& value, const A& allocator) :
        allocator_{allocator},
        size_{size},
        capacity_{size},
        data_{std::allocator_traits<A>::allocate(allocator_, size)}
    {
        T* begin = data_;
        T* end = data_ + size_;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            std::allocator_traits<A>::construct(allocator_, ptr, value);
        }
    }

    template <typename T, typename A>
    template <std::input_iterator I>
    constexpr vector<T, A>::vector(I first, I last, const A& allocator) : vector(allocator)
    {
        for (auto i = first; i != last; ++i) {
            push_back(*i);
        }
    }

    template <typename T, typename A>
    constexpr vector<T, A>::vector(std::initializer_list<T> init_list, const A& allocator) :
        vector(std::begin(init_list), std::end(init_list), allocator)
    {

    }

    // Rule of 5
    template <typename T, typename A>
    constexpr vector<T, A>::vector(const vector& other) :
        allocator_{std::allocator_traits<A>::select_on_container_copy_construction(other.allocator_)},
        size_{other.size_},
        capacity_{other.size_}, // vector does not have to copy capacity
        data_{std::allocator_traits<A>::allocate(allocator_, other.size_)}
    {
        for (std::size_t i = 0; i < size_; i += 1) {
            std::allocator_traits<A>::construct(allocator_, data_ + i, other.data_[i]);
        }
    }

    // Maybe reimplement for better performance?
    template <typename T, typename A>
    constexpr vector<T, A>& vector<T, A>::operator= (vector other)
    {
        this->swap(other);
        return *this;
    }

    template <typename T, typename A>
    constexpr vector<T, A>::vector(vector&& other) noexcept :
        allocator_{std::move(other.allocator_)},
        size_{other.size_},
        capacity_{other.capacity_},
        data_{other.data_}
    {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    template <typename T, typename A>
    constexpr vector<T, A>& vector<T, A>::operator= (vector&& other)
    {
        this->swap(vector<T, A>(std::move(other)));
        return *this;
    }

    template <typename T, typename A>
    vector<T, A>::~vector()
    {
        T* begin = data_;
        T* end = data_ + size_;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            std::allocator_traits<A>::destroy(allocator_, ptr);
        }

        std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::swap(vector& other) noexcept
    {
        using std::swap;
        swap(this->allocator_, this->allocator_);
        swap(this->size_, other.size_);
        swap(this->capacity_, other.capacity_);
        swap(this->data_, other.data_);
    }
    
    // Propery access
    template <typename T, typename A>
    constexpr bool vector<T, A>::empty() const noexcept
    {
        return true;
    }
    
    template <typename T, typename A>
    constexpr vector<T, A>::size_type vector<T, A>::size() const noexcept
    {
        return size_;
    }

    template <typename T, typename A>
    constexpr vector<T, A>::size_type vector<T, A>::max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }
    
    template <typename T, typename A>
    constexpr vector<T, A>::size_type vector<T, A>::capacity() const noexcept
    {
        return capacity_;
    }

    template <typename T, typename A>
    constexpr T* vector<T, A>::data() noexcept
    {
        return data_;
    }

    template <typename T, typename A>
    constexpr const T* vector<T, A>::data() const noexcept
    {
        return data_;
    }

    // Comparisons
    template <typename T, typename A>
    constexpr bool vector<T, A>::operator== (const vector& other) const
    {
        if (size() != other.size()) {
            return false;
        }

        for (size_type i = 0; i < size(); i += 1) {
            if ((*this)[i] != other[i]) {
                return false;
            }
        }

        return true;
    }

    template <typename T, typename A>
    constexpr auto vector<T, A>::operator<=> (const vector& other) const
    {
        if constexpr (std::three_way_comparable<T>) {
            for (size_type i = 0; i < std::min(size(), other.size()); i += 1) {
                if (auto comparison_result = (*this)[i] <=> other[i]; comparison_result != 0) {
                    return comparison_result;
                }
            }

            using ordering = std::compare_three_way_result_t<T>;
            return static_cast<ordering>(size() <=> other.size());
        } else {
            static_assert(requires (const T& a, const T& b)
            {
                { a == b } -> std::convertible_to<bool>;
                { a < b } -> std::convertible_to<bool>;
            });

            for (size_type i = 0; i < std::min(size(), other.size()); i += 1) {
                if ((*this)[i] < other[i]) {
                    return std::weak_ordering::less;
                }

                if (other[i] < (*this)[i]) {
                    return std::weak_ordering::greater;
                }
            }

            return std::weak_order(size(), other.size());
        }
    }

    // Element access
    template <typename T, typename A>
    constexpr vector<T, A>::reference vector<T, A>::operator[] (size_type index)
    {
        return data_[index];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reference vector<T, A>::operator[] (size_type index) const
    {
        return data_[index];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::reference vector<T, A>::at(size_type index)
    {
        if (index >= size()) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reference vector<T, A>::at(size_type index) const
    {
        if (index >= size()) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::reference vector<T, A>::front()
    {
        return (*this)[0];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reference vector<T, A>::front() const
    {
        return (*this)[0];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::reference vector<T, A>::back()
    {
        return (*this)[size() - 1];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reference vector<T, A>::back() const
    {
        return (*this)[size() - 1];
    }

    // Adding/removing elements
    template <typename T, typename A>
    constexpr void vector<T, A>::push_back(const T& value)
    {
        if (is_memory_filled()) {
            grow();
        }

        std::allocator_traits<A>::construct(allocator_, data_ + size_, value);
        size_ += 1;
    }  

    template <typename T, typename A>
    constexpr void vector<T, A>::push_back(T&& value)
    {
        if (is_memory_filled()) {
            grow();
        }
        
        std::allocator_traits<A>::construct(allocator_, data_ + size_, value); // Maybe (value)?
        size_ += 1;
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::pop_back()
    {
        size_ -= 1;
        std::allocator_traits<A>::destroy(allocator_, data_ + size_);
    }

    template <typename T, typename A>
    template <typename ... Args>
    constexpr vector<T, A>::reference vector<T, A>::emplace_back(Args&& ... args)
    {
        if (is_memory_filled()) {
            grow();
        }
        
        std::allocator_traits<A>::construct(allocator_, data_ + size_, std::forward<Args>(args) ...);
        size_ += 1;

        return back();
    }

    // Size/capacity modification
    template <typename T, typename A>
    constexpr void vector<T, A>::clear() noexcept
    {
        T* begin = data_;
        T* end = data_ + size_;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            std::allocator_traits<A>::destroy(allocator_, ptr);
        }

        size_ = 0;
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::resize(size_type new_size)
    {
        if (new_size > size()) {
            if (new_size > capacity()) {
                // Move data to new memory location
                const std::size_t new_capacity = new_size;
                T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);
                for (std::size_t i = 0; i < size(); i += 1) {
                    std::allocator_traits<A>::construct(allocator_, new_data + i, std::move(data_[i]));
                }

                // Clear old memory
                for (std::size_t i = 0; i < size(); i += 1) {
                    std::allocator_traits<A>::destroy(allocator_, data_ + i);
                }
                std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);
                data_ = new_data;
                capacity_ = new_capacity;
            }

            // Default construct new elements
            for (int i = size(); i < new_size; i += 1) {
                std::allocator_traits<A>::construct(allocator_, data_ + i);
            }
            size_ = new_size;
        } else if (new_size < size()) {
            for (int i = new_size; i < size(); i += 1) {
                std::allocator_traits<A>::destroy(allocator_, data_ + i);
            }
            size_ = new_size;
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::resize(size_type new_size, const T& value)
    {
        if (new_size > size()) {
            if (new_size > capacity()) {
                // Move data to new memory location
                const std::size_t new_capacity = new_size;

                T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);
                for (std::size_t i = 0; i < size(); i += 1) {
                    std::allocator_traits<A>::construct(allocator_, new_data + i, std::move(data_[i]));
                }

                // Clear old memory
                for (std::size_t i = 0; i < size(); i += 1) {
                    std::allocator_traits<A>::destroy(allocator_, data_ + i);
                }
                std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

                data_ = new_data;
                capacity_ = new_capacity;
            }

            // Copy construct new elements
            for (int i = size(); i < new_size; i += 1) {
                std::allocator_traits<A>::construct(allocator_, data_ + i, value);
            }
            size_ = new_size;
        } else if (new_size < size()) {
            for (int i = new_size; i < size(); i += 1) {
                std::allocator_traits<A>::destroy(allocator_, data_ + i);
            }
            size_ = new_size;
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::reserve(size_type new_capacity)
    {
        if (new_capacity > capacity()) {
            // Move data to new memory location
            T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);
            for (std::size_t i = 0; i < size(); i += 1) {
                std::allocator_traits<A>::construct(allocator_, new_data + i, std::move(data_[i]));
            }

            // Clear old memory
            for (std::size_t i = 0; i < size(); i += 1) {
                std::allocator_traits<A>::destroy(allocator_, data_ + i);
            }
            std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::shrink_to_fit()
    {
        if (size() < capacity()) {
            // Move data to new memory location
            const std::size_t new_capacity = size();
            T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);
            for (std::size_t i = 0; i < size(); i += 1) {
                std::allocator_traits<A>::construct(allocator_, new_data + i, std::move(data_[i]));
            }

            // Clear old memory
            for (std::size_t i = 0; i < size(); i += 1) {
                std::allocator_traits<A>::destroy(allocator_, data_ + i);
            }
            std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    // Iterators
    template <typename T, typename A>
    constexpr vector<T, A>::iterator vector<T, A>::begin() noexcept
    {
        return iterator(data());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::iterator vector<T, A>::end() noexcept
    {
        return iterator(data() + size());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_iterator vector<T, A>::begin() const noexcept
    {
        return const_iterator(data_); // data() will not work because it returns const T*
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_iterator vector<T, A>::end() const noexcept
    {
        return const_iterator(data_ + size_);
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_iterator vector<T, A>::cbegin() const noexcept
    {
        return begin();
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_iterator vector<T, A>::cend() const noexcept
    {
        return end();
    }

    // Reverse iterators
    template <typename T, typename A>
    constexpr vector<T, A>::reverse_iterator vector<T, A>::rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::reverse_iterator vector<T, A>::rend() noexcept
    {
        return reverse_iterator(begin());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reverse_iterator vector<T, A>::rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reverse_iterator vector<T, A>::rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reverse_iterator vector<T, A>::crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reverse_iterator vector<T, A>::crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    // Private member functions
    template <typename T, typename A>
    bool vector<T, A>::is_memory_filled() const
    {
        return size() == capacity();
    }

    template <typename T, typename A>
    void vector<T, A>::grow()
    {
        // Calculate new capacity
        constexpr std::size_t growth_factor = 2;
        std::size_t new_capacity;
        if (capacity_ == 0) { // if we were to double it would remain 0
            new_capacity = 1;
        } else {
            new_capacity = capacity_ * growth_factor;
        }

        // Move data to new memory location
        T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);
        for (std::size_t i = 0; i < size(); i += 1) {
            std::allocator_traits<A>::construct(allocator_, new_data + i, std::move(data_[i]));
        }

        // Clear old memory
        for (std::size_t i = 0; i < size(); i += 1) {
            std::allocator_traits<A>::destroy(allocator_, data_ + i);
        }
        std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

        data_ = new_data;
        capacity_ = new_capacity;
    }
}

#endif /* VECTOR_HPP */
