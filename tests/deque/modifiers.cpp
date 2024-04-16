#include "doctest/doctest.h"
#include "toy_sdl/deque.hpp"

#include <string>

TEST_SUITE("Deque modifiers") {
    TEST_CASE("Pushing back values should increase size") {
        SUBCASE("Pushing back one value") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.push_back(1);
            const auto size_after = d.size();
        
            CHECK(size_after > size_before);
        }

        SUBCASE("Pushing back multiple values") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.push_back(1);
            d.push_back(2);
            d.push_back(3);
            const auto size_after = d.size();
        
            CHECK(size_after == size_before + 3);
        }
    }

    TEST_CASE("Push back should work for different reference types") {
        SUBCASE("Const reference") {
            my::deque<int> d;
            d.push_back(static_cast<const int&>(1));
        }

        SUBCASE("Rvalue reference") {
            my::deque<int> d;
            d.push_back(static_cast<int&&>(1));
        }
    }

    TEST_CASE("Push back should add elements") {
        SUBCASE("Pushing back one value") {
            my::deque<int> d;

            d.push_back(1);
        
            CHECK(d[0] == 1);
        }

        SUBCASE("Pushing back multiple values") {
            my::deque<int> d;

            d.push_back(1);
            d.push_back(2);
            d.push_back(3);
        
            CHECK(d[0] == 1);
            CHECK(d[1] == 2);
            CHECK(d[2] == 3);
        }
    }

    TEST_CASE("Emplace back should increase size") {
        SUBCASE("Emplacing back one value") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.emplace_back(1);
            const auto size_after = d.size();
        
            CHECK(size_after > size_before);
        }

        SUBCASE("Emplacing back multiple values") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.emplace_back(1);
            d.emplace_back(2);
            d.emplace_back(3);
            const auto size_after = d.size();
        
            CHECK(size_after == size_before + 3);
        }
    }

    TEST_CASE("Emplace back should work with different constructors") {
        SUBCASE("Simple constructor") {
            my::deque<int> d;
            d.emplace_back(1);
        }

        SUBCASE("Nontrivial constructor") {
            my::deque<std::string> d;
            d.emplace_back("Hello");
        }

        SUBCASE("Constructor with multiple parameters") {
            struct Point { int x; int y; };
            my::deque<Point> d;

            const auto size_before = d.size();
            d.emplace_back(1, 2);
        }
    }

    TEST_CASE("Emplace back should add elements") {
        SUBCASE("Emplace back one value") {
            my::deque<int> d;

            d.emplace_back(1);
        
            CHECK(d[0] == 1);
        }

        SUBCASE("Emplace back multiple values") {
            my::deque<int> d;

            d.emplace_back(1);
            d.emplace_back(2);
            d.emplace_back(3);
        
            CHECK(d[0] == 1);
            CHECK(d[1] == 2);
            CHECK(d[2] == 3);
        }
    }
}