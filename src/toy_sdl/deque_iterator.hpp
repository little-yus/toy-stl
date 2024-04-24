#ifndef TOY_SDL_ITERATOR_HPP
#define TOY_SDL_ITERATOR_HPP

#include <compare>
#include <iterator>

namespace my
{
    template <typename T>
    class deque_iterator
    {
    public:
        using iterator_concept = std::random_access_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using element_type = value_type;

        deque_iterator() = default;
        explicit deque_iterator();

        bool operator==(const deque_iterator<T>& other) const;
        std::strong_ordering operator<=> (const deque_iterator<T>& other) const;

        deque_iterator& operator++();
        deque_iterator operator++(int);

        deque_iterator& operator--();
        deque_iterator operator--(int);

        deque_iterator& operator+=(difference_type n);
        template<typename U>
        friend deque_iterator<U> operator+(deque_iterator<U> i, deque_iterator<U>::difference_type n);
        template<typename U>
        friend deque_iterator<U> operator+(deque_iterator<U>::difference_type n, deque_iterator<U> i);

        deque_iterator& operator-=(difference_type n);
        template<typename U>
        friend deque_iterator<U>::difference_type operator-(const deque_iterator<U>& a, const deque_iterator<U>& b);

        T& operator[](difference_type n) const;
        T& operator*() const;
        T* operator->() const;
    private:
        
    };

    template <typename T>
    deque_iterator<T>::deque_iterator()
    {
        // TODO
    }

    template <typename T>
    bool deque_iterator<T>::operator==(const deque_iterator<T>& other) const
    {
        // TODO
    }

    template<typename T>
    std::strong_ordering deque_iterator<T>::operator<=>(const deque_iterator<T>& other) const
    {
        // TODO
    }

    template <typename T>
    deque_iterator<T>& deque_iterator<T>::operator++()
    {
        // TODO
        return *this;
    }

    template <typename T>
    deque_iterator<T> deque_iterator<T>::operator++(int)
    {
        const auto copy = *this;
        ++ptr;
        return copy;
    }

    template <typename T>
    deque_iterator<T>& deque_iterator<T>::operator--()
    {
        // TODO
        return *this;
    }

    template <typename T>
    deque_iterator<T> deque_iterator<T>::operator--(int)
    {
        const auto copy = *this;
        --ptr;
        return copy;
    }

    template<typename T>
    deque_iterator<T>& deque_iterator<T>::operator+=(difference_type n)
    {
        // TODO
        return *this;
    }

    template <typename T>
    deque_iterator<T> operator+(deque_iterator<T> i, typename deque_iterator<T>::difference_type n)
    {
        i += n;
        return i;
    }

    template <typename T>
    deque_iterator<T> operator+(typename deque_iterator<T>::difference_type n, deque_iterator<T> i)
    {
        i += n;
        return i;
    }

    template<typename T>
    deque_iterator<T>& deque_iterator<T>::operator-=(difference_type n)
    {
        // TODO
        return *this;
    }

    template <typename T>
    deque_iterator<T> operator-(deque_iterator<T> i, typename deque_iterator<T>::difference_type n)
    {
        i -= n;
        return i;
    }

    template <typename T>
    deque_iterator<T>::difference_type operator-(const deque_iterator<T>& a, const deque_iterator<T>& b)
    {
        // TODO
    }

    template<typename T>
    T& deque_iterator<T>::operator[](difference_type n) const
    {
        // TODO
    }

    template <typename T>
    T& deque_iterator<T>::operator*() const
    {
        // TODO
    }

    template <typename T>
    T* deque_iterator<T>::operator->() const
    {
        // TODO
    }
}

#endif /* TOY_SDL_ITERATOR_HPP */
