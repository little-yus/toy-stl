#include "doctest/doctest.h"
#include "toy_sdl/deque.hpp"

TEST_SUITE("Deque modifiers") {
    TEST_CASE("push_back") {
        SUBCASE("Should increase size") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.push_back(1);
        
            CHECK(d.size() > size_before);
        }

        SUBCASE("Should work for const references") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.push_back(static_cast<const int&>(1));
        }

        SUBCASE("Should work for rvalue references") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.push_back(static_cast<int&&>(1));
        }
    }

    TEST_CASE("emplace_back") {
        SUBCASE("Should increase size") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.emplace_back(1);
        
            CHECK(d.size() > size_before);
        }

        SUBCASE("Should work for complex constructors") {
            struct Point { int x; int y; };
            my::deque<Point> d;

            const auto size_before = d.size();
            d.emplace_back(1, 2);
        }
    }
}