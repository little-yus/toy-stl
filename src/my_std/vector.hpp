#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>
#include <new>
#include <cassert>

namespace my
{
    template <typename T>
    class vector_iterator
    {
    public:
        using value_type = T;

        using reference = T&;
        using pointer = T*;
        using const_reference = const T&;
        using rvalue_reference = T&&;

        using difference_type = std::ptrdiff_t;

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


    template <typename I>
    class reverse_iterator
    {
    public:
        using iterator_type = I;
        using iterator_concept = std::random_access_iterator_tag; // If Iter models std::random_access_iterator, this is std::random_access_iterator_tag. Otherwise, this is std::bidirectional_iterator_tag
        using iterator_category = std::iterator_traits<I>::iterator_category; //If std::iterator_traits<Iter>::iterator_category models std::derived_from<std::random_access_iterator_tag>, this is std::random_access_iterator_tag. Otherwise, this is std::iterator_traits<Iter>::iterator_category
        
        using value_type = std::iter_value_t<I>;
        using reference = std::iter_reference_t<I>;

        using difference_type = std::iter_difference_t<I>;
        using pointer = std::iterator_traits<I>::pointer;

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


    template <typename T>
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
        using reverse_iterator = reverse_iterator<iterator>;

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

        reverse_iterator rbegin();
        reverse_iterator rend();

    private:
        bool is_memory_filled() const;
        void grow();

        std::size_t size_ {0};
        std::size_t capacity_ {0};
        T* data_ {nullptr};
    };

    // Constructors
    template <typename T>
    vector<T>::vector()
    {

    }

    template <typename T>
    vector<T>::vector(std::size_t size) :
        size_{size},
        capacity_{size},
        data_{static_cast<T*>(::operator new (sizeof(T) * size))}
    {
        T* begin = data_;
        T* end = data_ + size_;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            new (ptr) T();
        }
    }

    template <typename T>
    vector<T>::vector(std::size_t size, const T& value) :
        size_{size},
        capacity_{size},
        data_{static_cast<T*>(::operator new (sizeof(T) * size))}
    {
        T* begin = data_;
        T* end = data_ + size_;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            new (ptr) T(value);
        }
    }

    template <typename T>
    template <std::input_iterator I>
    vector<T>::vector(I first, I last) : vector()
    {
        for (auto i = first; i != last; ++i) {
            push_back(*i);
        }
    }

    template <typename T>
    vector<T>::vector(std::initializer_list<T> init_list) :
        vector(std::begin(init_list), std::end(init_list))
    {

    }

    template <typename T>
    vector<T>::vector(const vector& other) :
        size_{other.size_},
        capacity_{other.size_}, // vector does not have to copy capacity
        data_{static_cast<T*>(::operator new (sizeof(T) * other.size_))}
    {
        for (std::size_t i = 0; i < size_; i += 1) {
            new (data_ + i) T(other.data_[i]);
        }
    }

    // Maybe reimplement for better performance?
    template <typename T>
    vector<T>& vector<T>::operator= (vector other)
    {
        this->swap(other);
        return *this;
    }

    template <typename T>
    vector<T>::vector(vector&& other) :
        size_{other.size_},
        capacity_{other.capacity_},
        data_{other.data_}
    {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    template <typename T>
    vector<T>& vector<T>::operator= (vector&& other)
    {
        this->swap(vector<T>(std::move(other)));
        return *this;
    }

    template <typename T>
    vector<T>::~vector()
    {
        T* begin = data_;
        T* end = data_ + size_;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            ptr->~T();
        }

        ::operator delete(static_cast<void*>(data_)); // Not sure if this is ok
    }

    template <typename T>
    void vector<T>::swap(vector& other)
    {
        using std::swap;
        swap(this->size_, other.size_);
        swap(this->capacity_, other.capacity_);
        swap(this->data_, other.data_);
    }

    // Other functions
    template <typename T>
    bool vector<T>::empty()
    {
        return true;
    }

    template <typename T>
    std::size_t vector<T>::size() const
    {
        return size_;
    }

    template <typename T>
    std::size_t vector<T>::capacity() const
    {
        return capacity_;
    }

    template <typename T>
    T* vector<T>::data()
    {
        return data_;
    }

    template <typename T>
    const T* vector<T>::data() const
    {
        return data_;
    }

    // Comparisons
    template <typename T>
    bool vector<T>::operator== (const vector& other) const
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

    template <typename T>
    auto vector<T>::operator<=> (const vector& other) const
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

    template <typename T>
    T& vector<T>::operator[] (std::size_t index)
    {
        return data_[index];
    }

    template <typename T>
    const T& vector<T>::operator[] (std::size_t index) const
    {
        return data_[index];
    }

    template <typename T>
    T& vector<T>::at(std::size_t index)
    {
        if (index >= size()) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T>
    const T& vector<T>::at(std::size_t index) const
    {
        if (index >= size()) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T>
    T& vector<T>::front()
    {
        return (*this)[0];
    }

    template <typename T>
    const T& vector<T>::front() const
    {
        return (*this)[0];
    }

    template <typename T>
    T& vector<T>::back()
    {
        return (*this)[size() - 1];
    }

    template <typename T>
    const T& vector<T>::back() const
    {
        return (*this)[size() - 1];
    }

    template <typename T>
    void vector<T>::push_back(const T& value)
    {
        if (is_memory_filled()) {
            grow();
        }
        
        new (data_ + size_) T(value);
        size_ += 1;
    }

    template <typename T>
    void vector<T>::push_back(T&& value)
    {
        if (is_memory_filled()) {
            grow();
        }
        
        new (data_ + size_) T(value);
        size_ += 1;
    }

    template <typename T>
    void vector<T>::pop_back()
    {
        size_ -= 1;
        data_[size_].~T();
    }

    template <typename T>
    template <typename ... Args>
    T& vector<T>::emplace_back(Args&& ... args)
    {
        if (is_memory_filled()) {
            grow();
        }
        
        new (data_ + size_) T(std::forward<Args>(args) ...);
        size_ += 1;

        return back();
    }

    template <typename T>
    void vector<T>::clear()
    {
        T* begin = data_;
        T* end = data_ + size_;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            ptr->~T();
        }

        size_ = 0;
    }

    template <typename T>
    void vector<T>::resize(std::size_t new_size)
    {
        if (new_size > size()) {
            if (new_size > capacity()) {
                // Move data to new memory location
                const std::size_t new_capacity = new_size;
                T* new_data = static_cast<T*>(::operator new (sizeof(T) * new_capacity));
                for (std::size_t i = 0; i < size(); i += 1) {
                    new (new_data + i) T(std::move(data_[i]));
                }

                // Clear old memory
                for (std::size_t i = 0; i < size(); i += 1) {
                    data_[i].~T();
                }
                ::operator delete(static_cast<void*>(data_));
                data_ = new_data;
                capacity_ = new_capacity;
            }

            // Default construct new elements
            for (int i = size(); i < new_size; i += 1) {
                new (data_ + i) T();
            }
            size_ = new_size;
        } else if (new_size < size()) {
            for (int i = new_size; i < size(); i += 1) {
                data_[i].~T();
            }
            size_ = new_size;
        }
    }

    template <typename T>
    void vector<T>::resize(std::size_t new_size, const T& value)
    {
        if (new_size > size()) {
            if (new_size > capacity()) {
                // Move data to new memory location
                const std::size_t new_capacity = new_size;
                T* new_data = static_cast<T*>(::operator new (sizeof(T) * new_capacity));
                for (std::size_t i = 0; i < size(); i += 1) {
                    new (new_data + i) T(std::move(data_[i]));
                }

                // Clear old memory
                for (std::size_t i = 0; i < size(); i += 1) {
                    data_[i].~T();
                }
                ::operator delete(static_cast<void*>(data_));
                data_ = new_data;
                capacity_ = new_capacity;
            }

            // Copy construct new elements
            for (int i = size(); i < new_size; i += 1) {
                new (data_ + i) T(value);
            }
            size_ = new_size;
        } else if (new_size < size()) {
            for (int i = new_size; i < size(); i += 1) {
                data_[i].~T();
            }
            size_ = new_size;
        }
    }

    template <typename T>
    void vector<T>::reserve(std::size_t new_capacity)
    {
        if (new_capacity > capacity()) {
            // Move data to new memory location
            T* new_data = static_cast<T*>(::operator new (sizeof(T) * new_capacity));
            for (std::size_t i = 0; i < size(); i += 1) {
                new (new_data + i) T(std::move(data_[i]));
            }

            // Clear old memory
            ::operator delete(static_cast<void*>(data_));
            for (std::size_t i = 0; i < size(); i += 1) {
                data_[i].~T();
            }
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    template <typename T>
    void vector<T>::shrink_to_fit()
    {
        if (size() < capacity()) {
            // Move data to new memory location
            const std::size_t new_capacity = size();
            T* new_data = static_cast<T*>(::operator new (sizeof(T) * new_capacity));
            for (std::size_t i = 0; i < size(); i += 1) {
                new (new_data + i) T(std::move(data_[i]));
            }

            // Clear old memory
            for (std::size_t i = 0; i < size(); i += 1) {
                data_[i].~T();
            }
            ::operator delete(static_cast<void*>(data_));
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    template <typename T>
    vector<T>::iterator vector<T>::begin()
    {
        if (size() == 0) {
            return vector_iterator<T>(nullptr);
        } else {
            return vector_iterator<T>(data());
        }
    }

    template <typename T>
    vector<T>::iterator vector<T>::end()
    {
        if (size() == 0) {
            return vector_iterator<T>(nullptr);
        } else {
            return vector_iterator<T>(data() + size());
        }
    }

    template <typename T>
    vector<T>::reverse_iterator vector<T>::rbegin()
    {
        return reverse_iterator(end());
    }

    template <typename T>
    vector<T>::reverse_iterator vector<T>::rend()
    {
        return reverse_iterator(begin());
    }

    // Private member functions
    template <typename T>
    bool vector<T>::is_memory_filled() const
    {
        return size() == capacity();
    }

    template <typename T>
    void vector<T>::grow()
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
        T* new_data = static_cast<T*>(::operator new (sizeof(T) * new_capacity));
        for (std::size_t i = 0; i < size(); i += 1) {
            new (new_data + i) T(std::move(data_[i]));
        }

        // Clear old memory
        for (std::size_t i = 0; i < size(); i += 1) {
            data_[i].~T();
        }
        ::operator delete(static_cast<void*>(data_));
        data_ = new_data;
        capacity_ = new_capacity;
    }
}

#endif /* VECTOR_HPP */
