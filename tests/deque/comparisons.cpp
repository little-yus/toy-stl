#include "doctest/doctest.h"
#include "toy_sdl/deque.hpp"

TEST_SUITE("Deque comparison operators") {
    TEST_CASE("Equality should work properly for different sizes") {
        SUBCASE("Both vectors empty") {
            my::deque<int> a;
            my::deque<int> b;
            REQUIRE(a.empty());
            REQUIRE(b.empty());

            CHECK(a == b);
        }

        SUBCASE("Different sizes") {
            my::deque<int> a;
            my::deque<int> b = {1, 2, 3};
            REQUIRE(a.size() != b.size());

            CHECK_FALSE(a == b);
        }

        SUBCASE("Same sizes, different elements") {
            my::deque<int> a = { 1, 2, 3 };
            my::deque<int> b = { 4, 5, 6 };
            REQUIRE(a.size() == b.size());

            CHECK_FALSE(a == b);
        }

        SUBCASE("Same sizes, same elements") {
            my::deque<int> a = { 1, 2, 3 };
            my::deque<int> b = { 1, 2, 3 };

            CHECK(a == b);
        }
    }

    TEST_CASE("Equality should satisfy properties of equivalence relation") {
        SUBCASE("Reflexivity") {
            my::deque<int> a = { 1, 2, 3 };

            CHECK(a == a);
        }

        SUBCASE("Symmetry") {
            my::deque<int> a = { 1, 2, 3 };
            my::deque<int> b = { 1, 2, 3 };

            REQUIRE(a == b);
            CHECK(b == a);
        }

        SUBCASE("Transitivity") {
            my::deque<int> a = { 1, 2, 3 };
            my::deque<int> b = { 1, 2, 3 };
            my::deque<int> c = { 1, 2, 3 };

            REQUIRE(a == b);
            REQUIRE(b == c);
            CHECK(a == c);
        }
    }

    TEST_CASE("Relational operators should work correctly for different sizes") {
        SUBCASE("Both vectors are empty") {
            my::deque<int> a;
            my::deque<int> b;
            REQUIRE(a.empty());
            REQUIRE(b.empty());
            
            CHECK_FALSE(a < b);
            CHECK_FALSE(b > a);
        }

        SUBCASE("One vector is empty") {
            my::deque<int> a;
            my::deque<int> b { 1, 2, 3 };
            REQUIRE(a.size() == 0);
            
            CHECK(a < b);
            CHECK(b > a);
        }

        SUBCASE("Same size, common prefix") {
            my::deque<int> a { 1, 1 };
            my::deque<int> b { 1, 2 };
            REQUIRE(a.size() == b.size());
            
            CHECK(a < b);
            CHECK(b > a);
        }

        SUBCASE("Same size, no common prefix") {
            my::deque<int> a { 1, 1 };
            my::deque<int> b { 2, 2 };
            REQUIRE(a.size() == b.size());
            
            CHECK(a < b);
            CHECK(b > a);
        }

        SUBCASE("Different size, one is prefix of other") {
            my::deque<int> a { 1 };
            my::deque<int> b { 1, 2, 3 };
            REQUIRE(a.size() != b.size());
            
            CHECK(a < b);
            CHECK(b > a);
        }

        SUBCASE("Different size, not prefixes of each other") {
            my::deque<int> a { 1, 2, 3 };
            my::deque<int> b { 2 };
            REQUIRE(a.size() != b.size());
            
            CHECK(a < b);
            CHECK(b > a);
        }
    }

    TEST_CASE("Relational operators should satisfy requirements of strict partial order")
    {
        SUBCASE("Antireflexivity") {
            my::deque<int> a { 1, 2, 3 };

            CHECK_FALSE(a < a);
        }

        SUBCASE("Asymmetry") {
            my::deque<int> a { 1, 2, 3 };
            my::deque<int> b { 2, 3, 4 };

            REQUIRE(a < b);
            CHECK_FALSE(b < a);
        }

        SUBCASE("Transitivity") {
            my::deque<int> a { 1, 2, 3 };
            my::deque<int> b { 2, 3, 4 };
            my::deque<int> c { 3, 4, 5 };

            REQUIRE(a < b);
            REQUIRE(b < c);
            CHECK(a < c);
        }
    }
}