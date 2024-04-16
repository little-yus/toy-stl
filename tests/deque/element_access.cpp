#include "doctest/doctest.h"
#include "toy_sdl/deque.hpp"

TEST_SUITE("Deque element access") {
    TEST_CASE("Accessing elements using subscript should return correct values") {
        my::deque<int> deq;

        deq.push_back(1);
        deq.push_back(2);
        deq.push_back(3);

        CHECK(deq[0] == 1);
        CHECK(deq[1] == 2);
        CHECK(deq[2] == 3);
    }

    TEST_CASE("Accessing elements using function at should return correct values") {
        my::deque<int> deq;

        deq.push_back(1);
        deq.push_back(2);
        deq.push_back(3);

        CHECK(deq.at(0) == 1);
        CHECK(deq.at(1) == 2);
        CHECK(deq.at(2) == 3);
    }

    TEST_CASE("Accessing elements in valid range should not throw") {
        my::deque<int> deq;

        deq.push_back(1);
        deq.push_back(2);
        deq.push_back(3);

        CHECK_NOTHROW(deq.at(0));
        CHECK_NOTHROW(deq.at(1));
        CHECK_NOTHROW(deq.at(2));
    }

    TEST_CASE("Trying to access out of range using at should throw exception") {
        SUBCASE("Empty deque") {
            my::deque<int> deq;

            CHECK_THROWS_AS(deq.at(0), std::out_of_range);
        }
        
        SUBCASE("Filled deque") {
            my::deque<int> deq;

            deq.push_back(1);
            deq.push_back(2);
            deq.push_back(3);

            CHECK_THROWS_AS(deq.at(-1), std::out_of_range);
            CHECK_THROWS_AS(deq.at(deq.size()), std::out_of_range);
        }
    }

    TEST_CASE("Accessing front element should return correct value") {
        my::deque<int> deq;

        deq.push_back(1);
        deq.push_back(2);
        deq.push_back(3);

        CHECK(deq.front() == 1);
    }

    TEST_CASE("Accessing back element should return correct value") {
        my::deque<int> deq;

        deq.push_back(1);
        deq.push_back(2);
        deq.push_back(3);

        CHECK(deq.back() == 3);
    }
}