#include "doctest/doctest.h"
#include "toy_stl/deque.hpp"

#include <array>

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

    TEST_CASE("Move constructor should create deque of the same size") {
        SUBCASE("Empty deque") {
            my::deque<int> a;
            REQUIRE(a.empty());

            my::deque<int> b(std::move(a));

            CHECK(b.empty());
        }

        SUBCASE("Non-empty deque") {
            my::deque<int> a;
            a.push_back(1);
            a.push_back(2);
            a.push_back(3);
            REQUIRE(!a.empty());

            const auto size_before_move = a.size();
            my::deque<int> b(std::move(a));

            CHECK(b.size() == size_before_move);
        }
    }

    TEST_CASE("Move constructor should create deque with the same elements") {
        my::deque<int> a;
        a.push_back(1);
        a.push_back(2);
        a.push_back(3);

        my::deque<int> b(std::move(a));

        CHECK(b[0] == 1);
        CHECK(b[1] == 2);
        CHECK(b[2] == 3);
    }

    TEST_CASE("Move assignment should create deque of same size") {
        SUBCASE("Empty deque") {
            my::deque<int> a;
            REQUIRE(a.empty());
            
            my::deque<int> b;
            b = std::move(a);

            CHECK(b.empty());
        }

        SUBCASE("Non-empty deque") {
            my::deque<int> a;
            a.push_back(1);
            a.push_back(2);
            a.push_back(3);
            REQUIRE(!a.empty());
            
            const auto size_before_move = a.size();
            my::deque<int> b;
            b = std::move(a);

            CHECK(b.size() == size_before_move);
        }
    }

    TEST_CASE("Move assignment should create deque with the same elements") {
        my::deque<int> a;
        a.push_back(1);
        a.push_back(2);
        a.push_back(3);

        my::deque<int> b;
        b = std::move(a);

        CHECK(b[0] == 1);
        CHECK(b[1] == 2);
        CHECK(b[2] == 3);
    }

    TEST_CASE("Constructor with size should create correct deque") {
        SUBCASE("New deque has correct size") {
            my::deque<int> deq(5);

            CHECK(deq.size() == 5);
        }

        SUBCASE("New deque has correct elements") {
            my::deque<int> deq(5);

            CHECK(deq[0] == 0);
            CHECK(deq[1] == 0);
            CHECK(deq[2] == 0);
            CHECK(deq[3] == 0);
            CHECK(deq[4] == 0);
        }
    }

    TEST_CASE("Constructor with size and value should create correct deque") {
        SUBCASE("New deque has correct size") {
            my::deque<int> deq(5, 123);

            CHECK(deq.size() == 5);
        }

        SUBCASE("New deque has correct elements") {
            my::deque<int> deq(5, 123);

            CHECK(deq[0] == 123);
            CHECK(deq[1] == 123);
            CHECK(deq[2] == 123);
            CHECK(deq[3] == 123);
            CHECK(deq[4] == 123);
        }
    }

    TEST_CASE("Constructor with iterator pair should create correct deque") {
        SUBCASE("Constructing from empty range") {
            std::array<int, 3> arr = { 1, 2, 3 };

            my::deque<int> deq(arr.begin(), arr.begin());

            CHECK(deq.empty());
        }

        SUBCASE("Constructing from non-empty range") {
            std::array<int, 3> arr = { 1, 2, 3 };

            my::deque<int> deq(arr.begin(), arr.end());

            CHECK(deq[0] == 1);
            CHECK(deq[1] == 2);
            CHECK(deq[2] == 3);
        }
    }

    TEST_CASE("Constructor with initializer list should create correct deque") {
        SUBCASE("Empty list") {
            my::deque<int> deq({});

            CHECK(deq.empty());
        }

        SUBCASE("Non-empty list") {
            my::deque<int> deq({ 1, 2, 3 });

            CHECK(deq[0] == 1);
            CHECK(deq[1] == 2);
            CHECK(deq[2] == 3);
        }
    }
}