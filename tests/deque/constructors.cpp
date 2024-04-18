#include "doctest/doctest.h"
#include "toy_sdl/deque.hpp"

TEST_SUITE("Deque constructors") {
    TEST_CASE("Default constructor should create empty deque") {
        my::deque<int> d;
        
        CHECK(d.empty());
    }

    TEST_CASE("Constructor taking only allocator should create empty deque") {
        my::deque<int> d(std::allocator<int> {});
        
        CHECK(d.empty());
    }

    TEST_CASE("Copy constructor should create deque of same size") {
        SUBCASE("Empty deque") {
            my::deque<int> a;
            REQUIRE(a.empty());

            my::deque<int> b(a);

            CHECK(b.empty());
        }

        SUBCASE("Non-empty deque") {
            my::deque<int> a;
            a.push_back(1);
            a.push_back(2);
            a.push_back(3);
            REQUIRE(!a.empty());

            my::deque<int> b(a);

            CHECK(b.size() == a.size());
        }
    }

    TEST_CASE("Copy constructor should create deque with the same elements") {
        my::deque<int> a;
        a.push_back(1);
        a.push_back(2);
        a.push_back(3);

        my::deque<int> b(a);

        CHECK(b[0] == a[0]);
        CHECK(b[1] == a[1]);
        CHECK(b[2] == a[2]);
    }

    TEST_CASE("Copy assignment should create deque of same size") {
        SUBCASE("Empty deque") {
            my::deque<int> a;
            REQUIRE(a.empty());
            
            my::deque<int> b;
            b = a;

            CHECK(b.empty());
        }

        SUBCASE("Non-empty deque") {
            my::deque<int> a;
            a.push_back(1);
            a.push_back(2);
            a.push_back(3);
            REQUIRE(!a.empty());
            
            my::deque<int> b;
            b = a;

            CHECK(b.size() == a.size());
        }
    }

    TEST_CASE("Copy assignment should create deque with the same elements") {
        my::deque<int> a;
        a.push_back(1);
        a.push_back(2);
        a.push_back(3);

        my::deque<int> b;
        b = a;

        CHECK(b[0] == a[0]);
        CHECK(b[1] == a[1]);
        CHECK(b[2] == a[2]);
    }
}