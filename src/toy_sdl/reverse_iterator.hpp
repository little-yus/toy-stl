#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP

#include <type_traits>
#include <concepts>

namespace my
{
    template <typename I>
    class reverse_iterator
    {
    public:
        using iterator_type = I;
        using iterator_concept = std::conditional_t<
            std::random_access_iterator<iterator_type>,
            std::random_access_iterator_tag,
            std::bidirectional_iterator_tag
        >;
        using iterator_category = std::conditional_t<
            std::derived_from<
                typename std::iterator_traits<I>::iterator_category,
                std::random_access_iterator_tag
            >,
            std::random_access_iterator_tag,
            typename std::iterator_traits<I>::iterator_category
        >;
        using value_type = std::iter_value_t<I>;
        using difference_type = std::iter_difference_t<I>;
        using pointer = std::iterator_traits<I>::pointer;
        using reference = std::iter_reference_t<I>;

        constexpr reverse_iterator() = default;
        constexpr explicit reverse_iterator(iterator_type iter);
        template <typename U>
            requires (!std::same_as<I, U> && std::convertible_to<const U&, I>)
        constexpr reverse_iterator(const reverse_iterator<U>& other);

        constexpr iterator_type base() const;

        constexpr reference operator* () const;
        constexpr pointer operator-> () const
            requires (std::is_pointer_v<I> || requires (const I i) { i.operator->(); });
        constexpr reference operator[] (difference_type n) const;

        template <typename T, typename U>
        friend constexpr bool operator== (const reverse_iterator<T>& a, const reverse_iterator<U>& b);
        template <typename T, typename U>
        friend constexpr decltype(auto) operator<=> (const reverse_iterator<T>& a, const reverse_iterator<U>& b);

        constexpr reverse_iterator& operator++ ();
        constexpr reverse_iterator operator++ (int);

        constexpr reverse_iterator& operator-- ();
        constexpr reverse_iterator operator-- (int);

        constexpr reverse_iterator& operator+= (difference_type n);
        constexpr reverse_iterator operator+ (difference_type n) const;
        template <typename U>
        friend constexpr reverse_iterator<U> operator+ (typename reverse_iterator<U>::difference_type n, const reverse_iterator<U>& i);

        constexpr reverse_iterator& operator-= (difference_type n);
        constexpr reverse_iterator operator- (difference_type n) const;
        template <typename T, typename U>
        friend constexpr decltype(auto) operator- (const reverse_iterator<T>& a, const reverse_iterator<U>& b);

    private:
        I iter{};
    };

    template <typename I>
    constexpr reverse_iterator<I>::reverse_iterator(I iter) : iter(iter)
    {

    }

    template <typename I>
    template <typename U>
        requires (!std::same_as<I, U> && std::convertible_to<const U&, I>)
    constexpr reverse_iterator<I>::reverse_iterator(const reverse_iterator<U>& other) : iter(other.iter)
    {

    }

    template <typename I>
    constexpr reverse_iterator<I>::iterator_type reverse_iterator<I>::base() const
    {
        return iter;
    }

    template <typename I>
    constexpr reverse_iterator<I>::reference reverse_iterator<I>::operator* () const
    {
        return *std::prev(iter);
    }

    template <typename I>
    constexpr reverse_iterator<I>::pointer reverse_iterator<I>::operator-> () const
        requires (std::is_pointer_v<I> || requires (const I i) { i.operator->(); })
    {
        if constexpr (std::is_pointer_v<I>) {
            return iter - 1;
        } else {
            return std::prev(iter).operator->();
        }
    }

    template<typename I>
    constexpr reverse_iterator<I>::reference reverse_iterator<I>::operator[] (difference_type n) const
    {
        return iter[-n - 1]; // Should give compilation error if iter does not support random access
    }

    template<typename T, typename U>
    constexpr bool operator== (const reverse_iterator<T>& a, const reverse_iterator<U>& b)
    {
        return a.iter == b.iter;
    }

    template<typename T, typename U>
    constexpr decltype(auto) operator<=> (const reverse_iterator<T>& a, const reverse_iterator<U>& b)
    {
        return b.iter <=> a.iter;
    }

    template <typename I>
    constexpr reverse_iterator<I>& reverse_iterator<I>::operator++ ()
    {
        --iter;
        return *this;
    }

    template <typename I>
    constexpr reverse_iterator<I> reverse_iterator<I>::operator++ (int)
    {
        const auto copy = *this;
        --iter;
        return copy;
    }

    template <typename I>
    constexpr reverse_iterator<I>& reverse_iterator<I>::operator-- ()
    {
        ++iter;
        return *this;
    }

    template <typename I>
    constexpr reverse_iterator<I> reverse_iterator<I>::operator-- (int)
    {
        const auto copy = *this;
        ++iter;
        return copy;
    }

    template<typename I>
    constexpr reverse_iterator<I>& reverse_iterator<I>::operator+= (difference_type n)
    {
        iter -= n;
        return *this;
    }

    template <typename I>
    constexpr reverse_iterator<I> reverse_iterator<I>::operator+ (difference_type n) const
    {
        auto tmp = *this;
        tmp += n; // logic is already handled by implementation of +=
        return tmp;
    }

    template <typename I>
    constexpr reverse_iterator<I> operator+ (typename reverse_iterator<I>::difference_type n, const reverse_iterator<I>& i)
    {
        auto tmp = i;
        tmp += n;
        return tmp;
    }

    template<typename I>
    constexpr reverse_iterator<I>& reverse_iterator<I>::operator-= (difference_type n)
    {
        iter += n;
        return *this;
    }

    template <typename I>
    constexpr reverse_iterator<I> reverse_iterator<I>::operator- (difference_type n) const
    {
        auto tmp = *this;
        tmp -= n; // logic is already handled by implementation of -=
        return tmp;
    }

    template <typename T, typename U>
    constexpr decltype(auto) operator- (const reverse_iterator<T>& a, const reverse_iterator<U>& b)
    {
        return b.iter - a.iter;
    }
}

#endif /* REVERSE_ITERATOR_HPP */
