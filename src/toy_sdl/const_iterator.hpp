#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP

#include <type_traits>
#include <concepts>
#include <iterator>

namespace my
{
    template <typename T, typename U>
    concept not_same_as = !std::same_as<T, U>;

    template <typename Iter>
    class basic_const_iterator;

    template <typename>
    constexpr bool is_basic_const_iterator_v = false;
    template <typename T>
    constexpr bool is_basic_const_iterator_v<basic_const_iterator<T>> = true;

    template <typename T>
    concept is_basic_const_iterator = is_basic_const_iterator_v<T>;

    template <typename T>
    concept is_not_basic_const_iterator = !is_basic_const_iterator_v<T>;

    // Not available in C++20 https://en.cppreference.com/w/cpp/iterator/iter_t
    template <std::indirectly_readable T> 
    using iter_const_reference_t = std::common_reference_t<
        const std::iter_value_t<T>&&,
        std::iter_reference_t<T>
    >;

    // https://en.cppreference.com/w/cpp/ranges/constant_range
    template <typename T>
    concept is_constant_iterator =
        std::input_iterator<T> &&
        std::same_as<iter_const_reference_t<T>, std::iter_reference_t<T>>;

    // I don't want to repeat every requires-clause, so definitions are inlined
    template <typename Iter>
    class basic_const_iterator
    {
    public:
        // Looks scary, but conceptually this selects correct iterator tag depending on corresponding concept
        using iterator_concept = std::conditional_t<
            std::contiguous_iterator<Iter>,
            std::contiguous_iterator_tag,
            std::conditional_t<
                std::random_access_iterator<Iter>,
                std::random_access_iterator_tag,
                std::conditional_t<
                    std::bidirectional_iterator<Iter>,
                    std::bidirectional_iterator_tag,
                    std::conditional_t<
                        std::forward_iterator<Iter>,
                        std::forward_iterator_tag,
                        std::input_iterator_tag
                    >
                >
            >
        >;
        using iterator_category = std::enable_if_t<
            std::forward_iterator<Iter>,
            typename std::iterator_traits<Iter>::iterator_category
        >;
        using value_type = std::iter_value_t<Iter>;
        using difference_type = std::iter_difference_t<Iter>;
    private:
        using reference = iter_const_reference_t<Iter>;
    
    public:
        constexpr basic_const_iterator() requires std::default_initializable<Iter> = default;
        constexpr basic_const_iterator(Iter iter) : current(std::move(iter)) { }
        template <std::convertible_to<Iter> U>
        constexpr basic_const_iterator(basic_const_iterator<U> other) : current(other.current) { }
        template <not_same_as<basic_const_iterator> T> requires std::convertible_to<T, Iter>
        constexpr basic_const_iterator(T&& iter) : current(std::forward<T>(iter)) { }

        constexpr const Iter& base() const& noexcept { return current; }
        constexpr Iter base() && { return std::move(current); }

        constexpr reference operator*() const { return *base(); }
        constexpr const auto* operator->() const
            requires // I just copied this from cppreference and don't understand this at all
                std::is_lvalue_reference_v<std::iter_reference_t<Iter>> &&
                std::same_as<
                    std::remove_cvref_t<std::iter_reference_t<Iter>>,
                    value_type
                >
        {
            if constexpr (std::contiguous_iterator<Iter>) {
                return std::to_address(base());
            } else {
                return std::addressof(*base());
            }
        }

        constexpr reference operator[](difference_type n) const
            requires std::random_access_iterator<Iter>
        {
            return base()[n];
        }

        // No forward_iterator requirement, so this can work for iterators without post increment
        constexpr basic_const_iterator& operator++()
        {
            ++current;
            return (*this);
        }

        constexpr void operator++(int)
        {
            ++(*this); // Function does not return anything, so i think this is ok
        }

        constexpr basic_const_iterator operator++(int) requires std::forward_iterator<Iter>
        {
            const auto copy = *this;
            ++(*this);
            return copy;
        }

        constexpr basic_const_iterator& operator--() requires std::bidirectional_iterator<Iter>
        {
            --current;
        }

        constexpr basic_const_iterator operator--(int) requires std::bidirectional_iterator<Iter>
        {
            const auto copy = *this;
            --(*this);
            return copy;
        }


        constexpr basic_const_iterator& operator+=(difference_type n) requires std::random_access_iterator<Iter>
        {
            current += n;
            return *this;
        }

        constexpr basic_const_iterator& operator-=(difference_type n) requires std::random_access_iterator<Iter>
        {
            current -= n;
            return *this;
        }


        // https://en.cppreference.com/w/cpp/iterator/basic_const_iterator/operator_constant_iterator
        // Names of exposition-only concepts not-a-const-iterator and constant-iterator are a bit confusing
        template <is_not_basic_const_iterator CI>
            requires
                is_constant_iterator<CI> &&
                std::convertible_to<Iter const&, CI>
        constexpr operator CI() const &
        {
            return current;
        }

        template <is_not_basic_const_iterator CI>
            requires
                is_constant_iterator<CI> &&
                std::convertible_to<Iter, CI>
        constexpr operator CI() &&
        {
            return std::move(current);
        }
        
        // Comparison with sentinel
        template <std::sentinel_for<Iter> S>
        constexpr bool operator==(const S& sentinel) const
        {
            return base() == sentinel;
        }

        // Comparisons in case Iter is not three_way_comparable
        constexpr bool operator<(const basic_const_iterator& other) const
            requires std::random_access_iterator<Iter>
        {
            return base() < other.base();
        }
        
        constexpr bool operator>(const basic_const_iterator& other) const
            requires std::random_access_iterator<Iter>
        {
            return base() > other.base();
        }

        constexpr bool operator<=(const basic_const_iterator& other) const
            requires std::random_access_iterator<Iter>
        {
            return base() <= other.base();
        }

        constexpr bool operator>=(const basic_const_iterator& other) const
            requires std::random_access_iterator<Iter>
        {
            return base() >= other.base();
        }
        
        // If Iter is three_way_comparable things are simpler
        constexpr auto operator<=>(const basic_const_iterator& other) const
            requires std::random_access_iterator<Iter> && std::three_way_comparable<Iter>
        {
            return base() <=> other.base();
        }
        

        // Same but for other iterator types
        template <not_same_as<basic_const_iterator> I>
        constexpr bool operator<(const I& other) const
            requires std::random_access_iterator<Iter> && std::totally_ordered_with<Iter, I>
        {
            return base() < other;
        }

        template <not_same_as<basic_const_iterator> I>
        constexpr bool operator>(const I& other) const
            requires std::random_access_iterator<Iter> && std::totally_ordered_with<Iter, I>
        {
            return base() > other;
        }

        template <not_same_as<basic_const_iterator> I>
        constexpr bool operator<=(const I& other) const
            requires std::random_access_iterator<Iter> && std::totally_ordered_with<Iter, I>
        {
            return base() <= other;
        }

        template <not_same_as<basic_const_iterator> I>
        constexpr bool operator>=(const I& other) const
            requires std::random_access_iterator<Iter> && std::totally_ordered_with<Iter, I>
        {
            return base() >= other;
        }

        template <not_same_as<basic_const_iterator> I>
        constexpr auto operator<=>(const I& other) const
            requires
                std::random_access_iterator<Iter> &&
                std::totally_ordered_with<Iter, I> &&
                std::three_way_comparable_with<Iter, I>
        {
            return base() <=> other;
        }
        

        // Non-member functions
        template <is_not_basic_const_iterator I>
        friend constexpr bool operator<(const I& x, const basic_const_iterator& y)
            requires std::random_access_iterator<Iter> && std::totally_ordered_with<Iter, I>
        {
            return x < y.base();
        }
        
        template <is_not_basic_const_iterator I>
        friend constexpr bool operator>(const I& x, const basic_const_iterator& y)
            requires std::random_access_iterator<Iter> && std::totally_ordered_with<Iter, I>
        {
            return x > y.base();
        }

        template <is_not_basic_const_iterator I>
        friend constexpr bool operator<=(const I& x, const basic_const_iterator& y)
            requires std::random_access_iterator<Iter> && std::totally_ordered_with<Iter, I>
        {
            return x <= y.base();
        }

        template <is_not_basic_const_iterator I>
        friend constexpr bool operator>=(const I& x, const basic_const_iterator& y)
            requires std::random_access_iterator<Iter> && std::totally_ordered_with<Iter, I>
        {
            return x >= y.base();
        }

        // friend constexpr bool operator<=>(const I& x, const basic_const_iterator& y) is missing on https://en.cppreference.com/w/cpp/iterator/basic_const_iterator/operator_cmp2
        

        friend constexpr basic_const_iterator operator+(const basic_const_iterator& i, difference_type n)
            requires std::random_access_iterator<Iter>
        {
            return basic_const_iterator(i.base() + n);
        }

        friend constexpr basic_const_iterator operator+(difference_type n, const basic_const_iterator& i)
            requires std::random_access_iterator<Iter>
        {
            return basic_const_iterator(n + i.base());
        }

        friend constexpr basic_const_iterator operator-(const basic_const_iterator& i, difference_type n)
            requires std::random_access_iterator<Iter>
        {
            return basic_const_iterator(i.base() - n);
        }
        
        template <std::sized_sentinel_for<Iter> S>
        constexpr difference_type operator-(const S& s) const
        {
            return base() - s;
        }

        template <is_not_basic_const_iterator S>
            requires std::sized_sentinel_for<S, Iter>
        friend constexpr difference_type operator-(const S& s, const basic_const_iterator& i)
        {
            return s - i.base();
        }

    private:
        Iter current{};
    };

    template <std::input_iterator I>
    using const_iterator = std::conditional_t<
        is_constant_iterator<I>,
        I,
        basic_const_iterator<I>
    >;
}

#endif /* CONST_ITERATOR_HPP */
