#include "doctest/doctest.h"
#include "toy_sdl/deque.hpp"

#include <ranges>

TEST_SUITE("Deque reverse iterators") {
    TEST_CASE("Iterators should be equality comparable") {
        SUBCASE("Empty deque") {
            my::deque<int> deq;

            CHECK(deq.rbegin() == deq.rend());
        }

        SUBCASE("Non-empty deque") {
            my::deque<int> deq = { 1, 2, 3 };

            CHECK(deq.rbegin() != deq.rend());
        }
    }

    TEST_CASE("Iterators should be ordered") {
        SUBCASE("Empty deque") {
            my::deque<int> deq;

            CHECK_FALSE(deq.rbegin() < deq.rend());
            CHECK_FALSE(deq.rbegin() > deq.rend());
        }

        SUBCASE("Non-empty deque") {
            my::deque<int> deq = { 1, 2, 3 };

            CHECK(deq.rbegin() < deq.rend());
            CHECK(deq.rend() > deq.rbegin());
        }
    }

    TEST_CASE("Dereferencing begin should return correct value") {
        my::deque<int> deq = { 1, 2, 3 };

        CHECK(*deq.rbegin() == deq.back());
    }

    TEST_CASE("Dereferencing end - 1 should return correct value") {
        my::deque<int> deq = { 1, 2, 3 };

        CHECK(*(deq.rend() - 1) == deq.front());
    }

    TEST_CASE("Difference between begin and end should be equal to deque size") {
        SUBCASE("Empty deque") {
            my::deque<int> deq;
            REQUIRE(deq.empty());

            CHECK(deq.rend() - deq.rbegin() == deq.size());
        }

        SUBCASE("Non-empty deque") {
            my::deque<int> deq = { 1, 2, 3 };

            CHECK(deq.rend() - deq.rbegin() == deq.size());
        }
    }

    TEST_CASE("Iterators should correctly handle advancing") {
        SUBCASE("Empty deque") {
            my::deque<int> deq;

            CHECK(deq.rbegin() + 0 == deq.rend());
            CHECK(0 + deq.rbegin() == deq.rend());
        }

        SUBCASE("Non-empty deque") {
            SUBCASE("Zero advance") {
                my::deque<int> deq = { 1, 2, 3 };

                CHECK(deq.rbegin() + 0 == deq.rbegin());
                CHECK(0 + deq.rbegin() == deq.rbegin());
            }
            
            SUBCASE("Maximum advance") {
                my::deque<int> deq = { 1, 2, 3 };

                CHECK(deq.rbegin() + deq.size() == deq.rend());
                CHECK(deq.size() + deq.rbegin() == deq.rend());
            }
            
            SUBCASE("Negative advance") {
                 my::deque<int> deq = { 1, 2, 3 };

                CHECK(deq.rend() - deq.size() == deq.rbegin());
            }
        }
    }

    TEST_CASE("Iterators should support range based for loop") {
        my::deque<int> deq = { 1, 2, 3 };

        std::size_t i = deq.size() - 1;
        for (auto& num : deq | std::views::reverse) {
            CHECK(num == deq[i]);
            --i;
        }
    }

    TEST_CASE("Iterators should satisfy all requirements of random access iterators") {
        CHECK(std::random_access_iterator<my::deque<int>::reverse_iterator>);
        CHECK(std::random_access_iterator<my::deque<std::string>::reverse_iterator>);
        CHECK(std::random_access_iterator<my::deque<my::deque<int>>::reverse_iterator>);
    }
}