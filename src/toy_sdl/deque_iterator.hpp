#ifndef TOY_SDL_DEQUE_ITERATOR_HPP
#define TOY_SDL_DEQUE_ITERATOR_HPP

#include "deque_data.hpp"

#include <compare>
#include <iterator>

namespace my
{
    template <typename Deq>
    class deque_iterator
    {
    private:
        using deque_data_type = typename Deq::deque_data_type;
        using size_type = typename Deq::size_type;

        constexpr static size_type block_size = Deq::block_size;

    public:
        using iterator_concept = std::random_access_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;

        using value_type = typename Deq::value_type;
        using difference_type = typename Deq::difference_type;
        using pointer = typename Deq::pointer;
        using reference = typename Deq::reference;
        using element_type = value_type;

        deque_iterator() = default;
        deque_iterator(deque_data_type* data, size_type index);

        bool operator==(const deque_iterator& other) const;
        std::strong_ordering operator<=>(const deque_iterator& other) const;

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

        reference operator[](difference_type n) const;
        reference operator*() const;
        pointer operator->() const;
    
    private:
        deque_data_type* data { nullptr };
        size_type index { 0 };
    };

    template <typename T>
    deque_iterator<T>::deque_iterator(deque_data_type* data, size_type index)
    {
        // TODO
    }

    template <typename T>
    bool deque_iterator<T>::operator==(const deque_iterator<T>& other) const
    {
        // TODO
    }

    template <typename T>
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
    deque_iterator<T>::reference deque_iterator<T>::operator[](difference_type n) const
    {
        // TODO
    }

    template <typename T>
    deque_iterator<T>::reference deque_iterator<T>::operator*() const
    {
        // TODO
    }

    template <typename T>
    deque_iterator<T>::pointer deque_iterator<T>::operator->() const
    {
        // TODO
    }
}

#endif /* TOY_SDL_DEQUE_ITERATOR_HPP */
