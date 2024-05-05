#ifndef TOY_SDL_ALGORITHM_HPP
#define TOY_SDL_ALGORITHM_HPP

#include <algorithm>
#include <compare>

namespace my
{
    // Implement synth_three_way
    // And mismatch + all_of maybe
    
    struct synth_three_way
    {
        template <typename T, typename U>
        constexpr auto operator()(const T& a, const U& b)
        {
            if constexpr (std::three_way_comparable_with<T, U>) {
                return a <=> b;
            } else {
                if (a < b) {
                    return std::weak_ordering::less;
                }

                if (b < a) {
                    return std::weak_ordering::greater;
                }

                return std::weak_ordering::equvivalent;
            } 
        }
    };

    template <typename T, typename U = T>
    using synth_three_way_result = decltype(synth_three_way{ }(std::declval<T&>(), std::declval<U&>()));

    template <class InputIt1, class InputIt2, class Cmp>
    constexpr auto lexicographical_compare_three_way(
        InputIt1 first1, InputIt1 last1,
        InputIt2 first2, InputIt2 last2,
        Cmp comp
    ) -> decltype(comp(*first1, *first2))
    {
        const auto& [mismatch1, mismatch2] = std::mismatch(
            first1, last1,
            first2, last2,
            [&](const auto& a, const auto& b) { return comp(a, b) == 0; }
        );

        // If any of the mismatch iterators are equal to corresponding end iterators then mismatch was not found
        if (mismatch1 == last1) {
            if (mismatch2 == last2) {
                // Ranges are equal
                return std::strong_ordering::equal;
            } else {
                // First range is prefix of second
                return std::strong_ordering::less;
            }
        } else if (mismatch2 == last2) {
            // Second range is prefix of first
            return std::strong_ordering::greater;
        }

        // If mismatch was found we compare it
        return comp(*mismatch1, *mismatch2);
    }
}

#endif /* TOY_SDL_ALGORITHM_HPP */
