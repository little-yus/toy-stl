#include "doctest/doctest.h"
#include "toy_sdl/deque.hpp"

#include <ranges>

TEST_SUITE("Deque const reverse iterators") {
    TEST_CASE("Const rbegin and crbegin should return same iterator") {
        my::deque<int> deq;

        CHECK(std::as_const(deq).rbegin() == deq.crbegin());
    }

    TEST_CASE("Const rend and crend should return same iterator") {
        my::deque<int> deq;

        CHECK(std::as_const(deq).rend() == deq.crend());
    }

    TEST_CASE("Iterators should be equality comparable") {
        SUBCASE("Empty deque") {
            const my::deque<int> deq;

            CHECK(deq.rbegin() == deq.rend());
        }

        SUBCASE("Non-empty deque") {
            const my::deque<int> deq = { 1, 2, 3 };

            CHECK(deq.rbegin() != deq.rend());
        }
    }

    TEST_CASE("Iterators should be ordered") {
        SUBCASE("Empty deque") {
            const my::deque<int> deq;

            CHECK_FALSE(deq.rbegin() < deq.rend());
            CHECK_FALSE(deq.rbegin() > deq.rend());
        }

        SUBCASE("Non-empty deque") {
            const my::deque<int> deq = { 1, 2, 3 };

            CHECK(deq.rbegin() < deq.rend());
            CHECK(deq.rend() > deq.rbegin());
        }
    }

    TEST_CASE("Dereferencing begin should return correct value") {
        const my::deque<int> deq = { 1, 2, 3 };

        CHECK(*deq.rbegin() == deq.back());
    }

    TEST_CASE("Dereferencing end - 1 should return correct value") {
        const my::deque<int> deq = { 1, 2, 3 };

        CHECK(*(deq.rend() - 1) == deq.front());
    }

    TEST_CASE("Difference between begin and end should be equal to deque size") {
        SUBCASE("Empty deque") {
            const my::deque<int> deq;
            REQUIRE(deq.empty());

            CHECK(deq.rend() - deq.rbegin() == deq.size());
        }

        SUBCASE("Non-empty deque") {
            const my::deque<int> deq = { 1, 2, 3 };

            CHECK(deq.rend() - deq.rbegin() == deq.size());
        }
    }

    TEST_CASE("Iterators should correctly handle advancing") {
        SUBCASE("Empty deque") {
            const my::deque<int> deq;

            CHECK(deq.rbegin() + 0 == deq.rend());
            CHECK(0 + deq.rbegin() == deq.rend());
        }

        SUBCASE("Non-empty deque") {
            SUBCASE("Zero advance") {
                const my::deque<int> deq = { 1, 2, 3 };

                CHECK(deq.rbegin() + 0 == deq.rbegin());
                CHECK(0 + deq.rbegin() == deq.rbegin());
            }
            
            SUBCASE("Maximum advance") {
                const my::deque<int> deq = { 1, 2, 3 };

                CHECK(deq.rbegin() + deq.size() == deq.rend());
                CHECK(deq.size() + deq.rbegin() == deq.rend());
            }
            
            SUBCASE("Negative advance") {
                const my::deque<int> deq = { 1, 2, 3 };

                CHECK(deq.rend() - deq.size() == deq.rbegin());
            }
        }
    }

    TEST_CASE("Iterators should support range based for loop") {
        const my::deque<int> deq = { 1, 2, 3 };

        std::size_t i = deq.size() - 1;
        for (const auto& num : deq | std::views::reverse) {
            CHECK(num == deq[i]);
            --i;
        }
    }

    TEST_CASE("Iterators should satisfy all requirements of random access iterators") {
        CHECK(std::random_access_iterator<my::deque<int>::const_reverse_iterator>);
        CHECK(std::random_access_iterator<my::deque<std::string>::const_reverse_iterator>);
        CHECK(std::random_access_iterator<my::deque<my::deque<int>>::const_reverse_iterator>);
    }
}