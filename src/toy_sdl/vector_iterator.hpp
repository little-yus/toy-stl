#ifndef TOY_SDL_VECTOR_ITERATOR_HPP
#define TOY_SDL_VECTOR_ITERATOR_HPP

#include <compare>
#include <iterator>

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

        vector_iterator& operator+= (difference_type n);
        template<typename U>
        friend vector_iterator<U> operator+ (vector_iterator<U> i, vector_iterator<U>::difference_type n);
        template<typename U>
        friend vector_iterator<U> operator+ (vector_iterator<U>::difference_type n, vector_iterator<U> i);

        vector_iterator& operator-= (difference_type n);
        template<typename U>
        friend vector_iterator<U>::difference_type operator- (const vector_iterator<U>& a, const vector_iterator<U>& b);

        T& operator[] (difference_type n) const;
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
    vector_iterator<T>& vector_iterator<T>::operator+= (difference_type n)
    {
        ptr += n;
        return *this;
    }

    template <typename T>
    vector_iterator<T> operator+ (vector_iterator<T> i, typename vector_iterator<T>::difference_type n)
    {
        i += n;
        return i;
    }

    template <typename T>
    vector_iterator<T> operator+ (typename vector_iterator<T>::difference_type n, vector_iterator<T> i)
    {
        i += n;
        return i;
    }

    template<typename T>
    vector_iterator<T>& vector_iterator<T>::operator-= (difference_type n)
    {
        ptr -= n;
        return *this;
    }

    template <typename T>
    vector_iterator<T> operator- (vector_iterator<T> i, typename vector_iterator<T>::difference_type n)
    {
        i -= n;
        return i;
    }

    template <typename T>
    vector_iterator<T>::difference_type operator- (const vector_iterator<T>& a, const vector_iterator<T>& b)
    {
        return a.ptr - b.ptr;
    }

    template<typename T>
    T& vector_iterator<T>::operator[] (difference_type n) const
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
}

#endif /* TOY_SDL_VECTOR_ITERATOR_HPP */
