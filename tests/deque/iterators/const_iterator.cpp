#include "doctest/doctest.h"
#include "toy_stl/deque.hpp"

TEST_SUITE("Deque const iterators") {
    TEST_CASE("Const begin and cbegin should return same iterator") {
        my::deque<int> deq;

        CHECK(std::as_const(deq).begin() == deq.cbegin());
    }

    TEST_CASE("Const end and cend should return same iterator") {
        my::deque<int> deq;

        CHECK(std::as_const(deq).end() == deq.cend());
    }

    // I will not test cbegin/cend separately, as these would be equvivalent to tests with const versions of begin/end
    // Also most of these test are just copy paste from ordinary iterator test, so less chance that I miss c somewhere

    TEST_CASE("Iterators should be equality comparable") {
        SUBCASE("Empty deque") {
            const my::deque<int> deq;

            CHECK(deq.begin() == deq.end());
        }

        SUBCASE("Non-empty deque") {
            const my::deque<int> deq = { 1, 2, 3 };

            CHECK(deq.begin() != deq.end());
        }
    }

    TEST_CASE("Iterators should be ordered") {
        SUBCASE("Empty deque") {
            const my::deque<int> deq;

            CHECK_FALSE(deq.begin() < deq.end());
            CHECK_FALSE(deq.begin() > deq.end());
        }

        SUBCASE("Non-empty deque") {
            const my::deque<int> deq = { 1, 2, 3 };

            CHECK(deq.begin() < deq.end());
            CHECK(deq.end() > deq.begin());
        }
    }

    TEST_CASE("Dereferencing begin should return correct value") {
        const my::deque<int> deq = { 1, 2, 3 };

        CHECK(*deq.begin() == 1);
        CHECK(*deq.begin() == deq.front());
    }

    TEST_CASE("Dereferencing end - 1 should return correct value") {
        const my::deque<int> deq = { 1, 2, 3 };

        CHECK(*(deq.end() - 1) == 3);
        CHECK(*(deq.end() - 1) == deq.back());
    }

    TEST_CASE("Difference between begin and end should be equal to deque size") {
        SUBCASE("Empty deque") {
            const my::deque<int> deq;
            REQUIRE(deq.empty());

            CHECK(deq.end() - deq.begin() == deq.size());
        }

        SUBCASE("Non-empty deque") {
            const my::deque<int> deq = { 1, 2, 3 };

            CHECK(deq.end() - deq.begin() == deq.size());
        }
    }

    TEST_CASE("Iterators should correctly handle advancing") {
        SUBCASE("Empty deque") {
            const my::deque<int> deq;

            CHECK(deq.begin() + 0 == deq.end());
            CHECK(0 + deq.begin() == deq.end());
        }

        SUBCASE("Non-empty deque") {
            SUBCASE("Zero advance") {
                const my::deque<int> deq = { 1, 2, 3 };

                CHECK(deq.begin() + 0 == deq.begin());
                CHECK(0 + deq.begin() == deq.begin());
            }
            
            SUBCASE("Maximum advance") {
                const my::deque<int> deq = { 1, 2, 3 };

                CHECK(deq.begin() + deq.size() == deq.end());
                CHECK(deq.size() + deq.begin() == deq.end());
            }
            
            SUBCASE("Negative advance") {
                const my::deque<int> deq = { 1, 2, 3 };

                CHECK(deq.end() - deq.size() == deq.begin());
            }
        }
    }

    TEST_CASE("Iterators should support range based for loop") {
        const my::deque<int> deq = { 1, 2, 3 };

        std::size_t i = 0;
        for (const auto& num : deq) {
            CHECK(num == deq[i]);
            i += 1;
        }
    }

    TEST_CASE("Iterators should satisfy all requirements of random access iterators") {
        CHECK(std::random_access_iterator<my::deque<int>::const_iterator>);
        CHECK(std::random_access_iterator<my::deque<std::string>::const_iterator>);
        CHECK(std::random_access_iterator<my::deque<my::deque<int>>::const_iterator>);
    }
}