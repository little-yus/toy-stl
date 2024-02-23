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
        vector();
        vector(std::size_t size);
        vector(std::size_t size, const T& value);
        template <std::input_iterator I>
        vector(I begin, I end);
        vector(std::initializer_list<T> init_list);

        // Rule of 5
        vector(const vector& other);
        vector& operator= (vector other);

        vector(vector&& other);
        vector& operator= (vector&& other);

        ~vector();

        void swap(vector& other);

        // Propery access
        bool empty();
        std::size_t size() const;
        std::size_t capacity() const;
        T* data();
        const T* data() const;

        // Comparison
        bool operator== (const vector& other) const;
        auto operator<=> (const vector& other) const;

        // Element access
        T& operator[] (std::size_t index);
        const T& operator[] (std::size_t index) const;

        T& at(std::size_t index);
        const T& at(std::size_t index) const;

        T& front();
        const T& front() const;

        T& back();
        const T& back() const;

        // Adding/removing elements
        void push_back(const T& value);
        void push_back(T&& value);
        void pop_back();
        template <typename ... Args>
        T& emplace_back(Args&& ... args);

        // Size/capacity modification
        void clear();
        void resize(std::size_t new_size);
        void resize(std::size_t new_size, const T& value);
        void reserve(std::size_t new_capacity);
        void shrink_to_fit();

        // Iterators
        iterator begin();
        iterator end();

        const_iterator begin() const;
        const_iterator end() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

        // Reverse iterators
        reverse_iterator rbegin();
        reverse_iterator rend();

        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;

        const_reverse_iterator crbegin() const;
        const_reverse_iterator crend() const;

    private:
        bool is_memory_filled() const;
        void grow();

        A allocator_ { };
        std::size_t size_ {0};
        std::size_t capacity_ {0};
        T* data_ {nullptr};
    };

    // Constructors
    template <typename T, typename A>
    vector<T, A>::vector()
    {

    }

    template <typename T, typename A>
    vector<T, A>::vector(std::size_t size) :
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
    vector<T, A>::vector(std::size_t size, const T& value) :
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
    vector<T, A>::vector(I first, I last) : vector()
    {
        for (auto i = first; i != last; ++i) {
            push_back(*i);
        }
    }

    template <typename T, typename A>
    vector<T, A>::vector(std::initializer_list<T> init_list) :
        vector(std::begin(init_list), std::end(init_list))
    {

    }

    template <typename T, typename A>
    vector<T, A>::vector(const vector& other) :
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
    vector<T, A>& vector<T, A>::operator= (vector other)
    {
        this->swap(other);
        return *this;
    }

    template <typename T, typename A>
    vector<T, A>::vector(vector&& other) :
        size_{other.size_},
        capacity_{other.capacity_},
        data_{other.data_}
    {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    template <typename T, typename A>
    vector<T, A>& vector<T, A>::operator= (vector&& other)
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
    void vector<T, A>::swap(vector& other)
    {
        using std::swap;
        swap(this->size_, other.size_);
        swap(this->capacity_, other.capacity_);
        swap(this->data_, other.data_);
    }

    // Other functions
    template <typename T, typename A>
    bool vector<T, A>::empty()
    {
        return true;
    }

    template <typename T, typename A>
    std::size_t vector<T, A>::size() const
    {
        return size_;
    }

    template <typename T, typename A>
    std::size_t vector<T, A>::capacity() const
    {
        return capacity_;
    }

    template <typename T, typename A>
    T* vector<T, A>::data()
    {
        return data_;
    }

    template <typename T, typename A>
    const T* vector<T, A>::data() const
    {
        return data_;
    }

    // Comparisons
    template <typename T, typename A>
    bool vector<T, A>::operator== (const vector& other) const
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
    auto vector<T, A>::operator<=> (const vector& other) const
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

    template <typename T, typename A>
    T& vector<T, A>::operator[] (std::size_t index)
    {
        return data_[index];
    }

    template <typename T, typename A>
    const T& vector<T, A>::operator[] (std::size_t index) const
    {
        return data_[index];
    }

    template <typename T, typename A>
    T& vector<T, A>::at(std::size_t index)
    {
        if (index >= size()) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T, typename A>
    const T& vector<T, A>::at(std::size_t index) const
    {
        if (index >= size()) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T, typename A>
    T& vector<T, A>::front()
    {
        return (*this)[0];
    }

    template <typename T, typename A>
    const T& vector<T, A>::front() const
    {
        return (*this)[0];
    }

    template <typename T, typename A>
    T& vector<T, A>::back()
    {
        return (*this)[size() - 1];
    }

    template <typename T, typename A>
    const T& vector<T, A>::back() const
    {
        return (*this)[size() - 1];
    }

    template <typename T, typename A>
    void vector<T, A>::push_back(const T& value)
    {
        if (is_memory_filled()) {
            grow();
        }

        std::allocator_traits<A>::construct(allocator_, data_ + size_, value);
        size_ += 1;
    }  

    template <typename T, typename A>
    void vector<T, A>::push_back(T&& value)
    {
        if (is_memory_filled()) {
            grow();
        }
        
        std::allocator_traits<A>::construct(allocator_, data_ + size_, value); // Maybe std::forward(value)?
        size_ += 1;
    }

    template <typename T, typename A>
    void vector<T, A>::pop_back()
    {
        size_ -= 1;
        std::allocator_traits<A>::destroy(allocator_, data_ + size_);
    }

    template <typename T, typename A>
    template <typename ... Args>
    T& vector<T, A>::emplace_back(Args&& ... args)
    {
        if (is_memory_filled()) {
            grow();
        }
        
        std::allocator_traits<A>::construct(allocator_, data_ + size_, std::forward<Args>(args) ...);
        size_ += 1;

        return back();
    }

    template <typename T, typename A>
    void vector<T, A>::clear()
    {
        T* begin = data_;
        T* end = data_ + size_;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            std::allocator_traits<A>::destroy(allocator_, ptr);
        }

        size_ = 0;
    }

    template <typename T, typename A>
    void vector<T, A>::resize(std::size_t new_size)
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
    void vector<T, A>::resize(std::size_t new_size, const T& value)
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
    void vector<T, A>::reserve(std::size_t new_capacity)
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
    void vector<T, A>::shrink_to_fit()
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

    template <typename T, typename A>
    vector<T, A>::iterator vector<T, A>::begin()
    {
        return iterator(data());
    }

    template <typename T, typename A>
    vector<T, A>::iterator vector<T, A>::end()
    {
        return iterator(data() + size());
    }

    template <typename T, typename A>
    vector<T, A>::const_iterator vector<T, A>::begin() const
    {
        return const_iterator(data_); // data() will not work because it returns const T*
    }

    template <typename T, typename A>
    vector<T, A>::const_iterator vector<T, A>::end() const
    {
        return const_iterator(data_ + size_);
    }

    template <typename T, typename A>
    vector<T, A>::const_iterator vector<T, A>::cbegin() const
    {
        return begin();
    }

    template <typename T, typename A>
    vector<T, A>::const_iterator vector<T, A>::cend() const
    {
        return end();
    }

    template <typename T, typename A>
    vector<T, A>::reverse_iterator vector<T, A>::rbegin()
    {
        return reverse_iterator(end());
    }

    template <typename T, typename A>
    vector<T, A>::reverse_iterator vector<T, A>::rend()
    {
        return reverse_iterator(begin());
    }

    template <typename T, typename A>
    vector<T, A>::const_reverse_iterator vector<T, A>::rbegin() const
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename A>
    vector<T, A>::const_reverse_iterator vector<T, A>::rend() const
    {
        return const_reverse_iterator(begin());
    }

    template <typename T, typename A>
    vector<T, A>::const_reverse_iterator vector<T, A>::crbegin() const
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename A>
    vector<T, A>::const_reverse_iterator vector<T, A>::crend() const
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
